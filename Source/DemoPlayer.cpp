#include "DemoPlayer.h"
#include <filesystem>
#include "Utils.h"

namespace fs = std::filesystem;

DemoPlayer::~DemoPlayer()
{
	if (_state == State::Playing)
		StopPlaying();
	else if (_state == State::Recording)
		StopRecording();
}

void DemoPlayer::SetCallbacks(RecorderSamplingFunc samplingFunc, PlayerSampleUpdaterFunc sampleUpdaterFunc)
{
	_samplingFunc = std::move(samplingFunc);
	_sampleUpdaterFunc = std::move(sampleUpdaterFunc);
}

const std::vector<std::string>& DemoPlayer::GetDemoList() const
{
	return _demoList;
}

void DemoPlayer::UpdateDemoList()
{
	std::string fullDirPathStr = GetFullPath("../Demos/");

	_demoList.clear();

	std::error_code ec;
	fs::path demoDirPath = fs::canonical(fullDirPathStr, ec);
	if (!ec)
	{
		for (auto& demoFile : fs::directory_iterator(demoDirPath))
			if (demoFile.path().extension() == ".demo")
				_demoList.push_back(demoFile.path().stem().string());
	}
}

void DemoPlayer::DeleteDemo(const char* demoName)
{
	std::string fileName = std::string("../Demos/") + demoName + ".demo";
	std::string fullPathStr = GetFullPath(fileName.c_str());

	std::error_code ec;
	if (fs::remove(fullPathStr, ec))
		UpdateDemoList();
}

void DemoPlayer::Update(float dt)
{
	if (_state == State::Playing)
	{
		PlayUpdate(dt);
	}
	else if (_state == State::Recording)
	{
		RecordUpdate(dt);
	}
}

bool DemoPlayer::StartRecording(const char* demoName, float freqHz)
{
	if (_state != State::Ready)
		return false;

	if (demoName == nullptr || *demoName == '\0')
		return false;

	if (_samplingFunc == nullptr)
		return false;

	std::string dirPath = GetFullPath("../Demos/");
	if (!fs::exists(dirPath))
	{
		std::error_code ec;
		if (!fs::create_directory(dirPath, ec))
			return false;
	}
	std::string fileName = dirPath + demoName + ".demo";

	_recordFile = fopen(fileName.c_str(), "wb");
	if (_recordFile == nullptr)
		return false;

	_samplePeriod = 1.0f / std::clamp(freqHz, 10.0f, 120.0f);
	_sampleTime = 0.0f;
	_currentTime = 0.0f;
	_state = State::Recording;

	// Write the first block with time stamp 0.
	_numSamplesWritten = 0;
	_samplingFunc();
	uint32_t endId = -1;
	fwrite(&endId, 4, 1, _recordFile);

	return true;
}

void DemoPlayer::RecordSample(int32_t id, RecCond recCond, size_t size, const void* data)
{
	if (_state == State::Recording)
	{
		if (recCond == RecCond::IfChanged)
		{
			auto it = _lastRecordedDataMap.find(id);
			if (it != _lastRecordedDataMap.end())
			{
				if (std::equal(it->second.begin(), it->second.end(), reinterpret_cast<const char*>(data)))
					return;
			}
		}

		// If this is the first sample in this block, write the current time before it.
		if (_numSamplesWritten == 0)
			fwrite(&_currentTime, 4, 1, _recordFile);

		fwrite(&id, 4, 1, _recordFile);
		int32_t size32 = static_cast<int32_t>(size);
		fwrite(&size32, 4, 1, _recordFile);
		fwrite(data, 1, size, _recordFile);
		const char* start = reinterpret_cast<const char*>(data);
		const char* end = reinterpret_cast<const char*>(data) + size;
		_lastRecordedDataMap[id] = std::vector(start, end);
		_numSamplesWritten++;
	}
	else if (_state == State::Ready)
	{
		const char* start = reinterpret_cast<const char*>(data);
		const char* end = reinterpret_cast<const char*>(data) + size;
		_initialDataMap[id] = std::vector(start, end);
	}
}

void DemoPlayer::PauseRecording()
{
}

void DemoPlayer::ResumeRecording()
{
}

void DemoPlayer::StopRecording()
{
	if (_state == State::Recording)
	{
		// Write the last empty block and close the file.
		fwrite(&_currentTime, 4, 1, _recordFile);
		int32_t endId = -2;
		fwrite(&endId, 4, 1, _recordFile);

		fclose(_recordFile);
		_recordFile = nullptr;

		_currentTime = 0.0f;
		_sampleTime = 0.0f;
		_samplePeriod = 0.0f;
		_lastRecordedDataMap.clear();
		_state = State::Ready;
	}
}

bool DemoPlayer::LoadDemo(const char* demoName)
{
	if (demoName == nullptr)
		return false;

	std::string fullDirPathStr = GetFullPath("../Demos/");
	fs::path demoPath(fullDirPathStr);
	demoPath.append(std::string(demoName) + ".demo");
	std::error_code ec;
	demoPath = fs::canonical(demoPath, ec);
	if (ec)
		return false;

	FILE* file = fopen(demoPath.string().c_str(), "rb");
	if (file == nullptr)
		return false;

	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	_data.resize(size);
	fseek(file, 0, SEEK_SET);
	fread(_data.data(), 1, size, file);
	fclose(file);

	_currentDataPtr = _data.data();

	return true;
}

void DemoPlayer::UnloadDemo()
{
	_data.clear();
	_currentDataPtr = nullptr;
}

bool DemoPlayer::StartPlaying()
{
	if (_state != State::Ready ||
		_data.empty() ||
		_sampleUpdaterFunc == nullptr)
	{
		return false;
	}

	_currentTime = 0.0f;
	_currentDataPtr = _data.data();
	_state = State::Playing;

	return true;
}

void DemoPlayer::StopPlaying()
{
	if (_state == State::Playing)
	{
		_state = State::Ready;
	}
}

void DemoPlayer::TakeSamplesSnapshot()
{
	if (_state == State::Ready)
	{
		_initialDataMap.clear();

		// Call sampling callback function to save the data to be reverted after
		// the playback has finished.
		if (_samplingFunc)
			_samplingFunc();
	}
}

void DemoPlayer::RestoreSamplesSnapshot()
{
	if (_state == State::Ready)
	{
		// Revert previously saved data.
		if (_sampleUpdaterFunc)
			for (const auto& [id, oldData] : _initialDataMap)
				_sampleUpdaterFunc(id, oldData.size(), oldData.data());

		_initialDataMap.clear();
	}
}

void DemoPlayer::PlayUpdate(float dt)
{
	_currentTime += dt;
	if (_currentDataPtr == nullptr)
		_currentDataPtr = _data.data();

	while (true)
	{
		float time = *reinterpret_cast<float*>(_currentDataPtr);
		if (time > _currentTime)
			break;

		_currentDataPtr += sizeof(float);

		while (true)
		{
			int32_t id = *reinterpret_cast<int32_t*>(_currentDataPtr);
			_currentDataPtr += sizeof(int32_t);
			if (id == -1)
			{
				// End of sample block.
				break;
			}
			else if (id == -2)
			{
				// End of demo.
				StopPlaying();
				return;
			}

			int32_t size = *reinterpret_cast<int32_t*>(_currentDataPtr);
			_currentDataPtr += sizeof(int32_t);

			_sampleUpdaterFunc(id, size, _currentDataPtr);

			_currentDataPtr += size;
		}
	}
}

void DemoPlayer::RecordUpdate(float dt)
{
	_currentTime += dt;
	_sampleTime += dt;
	if (_sampleTime >= _samplePeriod)
	{
		_sampleTime -= _samplePeriod;

		_numSamplesWritten = 0;
		_samplingFunc();

		if (_numSamplesWritten > 0)
		{
			// Write id -1 to mark the end of the sample block.
			int32_t endId = -1;
			fwrite(&endId, 4, 1, _recordFile);
		}
	}
}
