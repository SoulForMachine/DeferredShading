#ifndef _DEMO_PLAYER_H_
#define _DEMO_PLAYER_H_

#include <cstdint>
#include <cstddef>
#include <functional>
#include <vector>
#include <string>
#include <map>
#include <cstdio>

class DemoPlayer
{
public:
	using RecorderSamplingFunc = std::function<void()>;
	using PlayerSampleUpdaterFunc = std::function<void(int32_t, size_t, const void*)>;

	enum class State
	{
		Ready,
		Recording,
		Playing
	};

	enum class RecCond
	{
		Always,
		IfChanged,
	};

	~DemoPlayer();

	void SetCallbacks(RecorderSamplingFunc samplingFunc, PlayerSampleUpdaterFunc sampleUpdaterFunc);
	State GetState() const { return _state; }
	const std::vector<std::string>& GetDemoList() const;

	void UpdateDemoList();
	void DeleteDemo(const char* demoName);
	void Update(float dt);

	bool StartRecording(const char* demoName, float freqHz = 60.0f);
	void RecordSample(int32_t id, RecCond recCond, size_t size, const void* data);
	void PauseRecording();
	void ResumeRecording();
	void StopRecording();

	bool LoadDemo(const char* demoName);
	void UnloadDemo();
	bool StartPlaying();
	void StopPlaying();

	void TakeSamplesSnapshot();
	void RestoreSamplesSnapshot();

private:
	void PlayUpdate(float dt);
	void RecordUpdate(float dt);

	State _state = State::Ready;
	std::vector<std::string> _demoList;
	float _currentTime = 0.0f;
	float _sampleTime = 0.0f;
	float _samplePeriod = 0.0f;
	char* _currentDataPtr = nullptr;
	std::vector<char> _data;
	FILE* _recordFile = nullptr;
	std::map<int32_t, std::vector<char>> _lastRecordedDataMap;
	std::map<int32_t, std::vector<char>> _initialDataMap;
	int _numSamplesWritten = 0;
	RecorderSamplingFunc _samplingFunc;
	PlayerSampleUpdaterFunc _sampleUpdaterFunc;
};

#endif // _DEMO_PLAYER_H_
