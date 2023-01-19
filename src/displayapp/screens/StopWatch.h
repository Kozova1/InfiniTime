#pragma once

#include "displayapp/screens/Screen.h"
#include <lvgl/lvgl.h>

#include <FreeRTOS.h>
#include "portmacro_cmsis.h"

#include "systemtask/SystemTask.h"

namespace Pinetime::Applications::Screens {

  enum class States { Init, Running, Halted };

  struct TimeSeparated_t {
    int mins;
    int secs;
    int hundredths;
  };

  class StopWatch : public Screen {
  public:
    StopWatch(DisplayApp* app, System::SystemTask& systemTask);
    ~StopWatch() override;
    void Refresh() override;

    void playPauseBtnEventHandler(lv_event_t event);
    void stopLapBtnEventHandler(lv_event_t event);
    bool OnButtonPushed() override;

    void Reset();
    void Start();
    void Pause();

    void RefreshOnce();
    void StyleRunning();
    void StyleHalt();
    void UpdateDisplay();

  private:
    Pinetime::System::SystemTask& systemTask;
    static constexpr int maxLapCount = 20;
    TickType_t laps[maxLapCount + 1];
    static constexpr int displayedLaps = 2;
    int lapsDone = 0;
    lv_obj_t *time, *msecTime, *btnPlayPause, *btnStopLap, *txtPlayPause, *txtStopLap;
    lv_obj_t* lapText;

    lv_task_t* taskRefresh;
  };
}
