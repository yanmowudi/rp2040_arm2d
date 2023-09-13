/****************************************************************************
*  Copyright 2021 Gorgon Meducer (Email:embedded_zhuoran@hotmail.com)       *
*                                                                           *
*  Licensed under the Apache License, Version 2.0 (the "License");          *
*  you may not use this file except in compliance with the License.         *
*  You may obtain a copy of the License at                                  *
*                                                                           *
*     http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                           *
*  Unless required by applicable law or agreed to in writing, software      *
*  distributed under the License is distributed on an "AS IS" BASIS,        *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. *
*  See the License for the specific language governing permissions and      *
*  limitations under the License.                                           *
*                                                                           *
****************************************************************************/
/*============================ INCLUDES ======================================*/
#include "pico/stdlib.h"
#include "perf_counter.h"
#include "bsp/bsp.h"

#include <stdio.h>

#include "RTE_Components.h"
#if defined(RTE_Compiler_EventRecorder) && defined(RTE_Compiler_IO_STDOUT_EVR)
#   include <EventRecorder.h>
#endif

#if defined(RTE_Script_PikaScript)
#   include "pikaScript.h"
#endif

#if defined(__RTE_ACCELERATION_ARM_2D__) || defined(RTE_Acceleration_Arm_2D)
#   include "arm_2d.h"
#   include "arm_2d_helper.h"
#   include "arm_2d_disp_adapters.h"
#   include "arm_2d_scene_0.h"
#   include "arm_2d_scene_1.h"
#   include "arm_2d_scene_2.h"
#   include "arm_2d_scene_3.h"
#   include "arm_2d_scene_4.h"
#   include "arm_2d_scene_5.h"
#endif

#include "arm_2d_scene_meter.h"
#include "arm_2d_scene_watch.h"

#if defined(RTE_Acceleration_Arm_2D_Extra_Benchmark)
#   include "arm_2d_benchmark.h"
#endif

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/

void scene_meter_loader(void) 
{
    arm_2d_scene_meter_init(&DISP0_ADAPTER);
}

void scene_watch_loader(void) 
{
    arm_2d_scene_watch_init(&DISP0_ADAPTER);
}


void scene0_loader(void) 
{
    arm_2d_scene0_init(&DISP0_ADAPTER);
}

void scene1_loader(void) 
{
    arm_2d_scene1_init(&DISP0_ADAPTER);
}

void scene2_loader(void) 
{
    arm_2d_scene2_init(&DISP0_ADAPTER);
}

void scene3_loader(void) 
{
    arm_2d_scene3_init(&DISP0_ADAPTER);
}

void scene4_loader(void) 
{
    arm_2d_scene4_init(&DISP0_ADAPTER);
}

void scene5_loader(void) 
{
    arm_2d_scene5_init(&DISP0_ADAPTER);
}

typedef void scene_loader_t(void);

static scene_loader_t * const c_SceneLoaders[] = {
    scene0_loader,
    scene1_loader,
    scene_meter_loader,
    scene3_loader,
    scene5_loader,
    scene4_loader,
    scene2_loader,
    scene_watch_loader,
};


/* load scene one by one */
void before_scene_switching_handler(void *pTarget,
                                    arm_2d_scene_player_t *ptPlayer,
                                    arm_2d_scene_t *ptScene)
{
    static uint_fast8_t s_chIndex = 0;

    if (s_chIndex >= dimof(c_SceneLoaders)) {
        s_chIndex = 0;
    }
    
    /* call loader */
    c_SceneLoaders[s_chIndex]();
    s_chIndex++;
}

void SysTick_Handler(void)
{

}

static void system_init(void)
{
    extern void SystemCoreClockUpdate();

    SystemCoreClockUpdate();
    /*! \note if you do want to use SysTick in your application, please use 
     *!       init_cycle_counter(true); 
     *!       instead of 
     *!       init_cycle_counter(false); 
     */
    init_cycle_counter(false);

#if defined(RTE_Compiler_EventRecorder) && defined(RTE_Compiler_IO_STDOUT_EVR)
    EventRecorderInitialize(0, 1);
#endif
    stdio_init_all();

    led_init();
    spilcd_init();
    //i80lcd_init();
}

int main(void) 
{
    system_init();

    __cycleof__("printf") {
        printf("Hello Pico-Template\r\n");
    }
    
#if defined(RTE_Script_PikaScript)
    pikaScriptInit();
#endif

#if defined( __PERF_COUNTER_COREMARK__ ) && __PERF_COUNTER_COREMARK__
    printf("\r\nRun Coremark 1.0...\r\n");
    coremark_main();
#endif



#if defined(__RTE_ACCELERATION_ARM_2D__) || defined(RTE_Acceleration_Arm_2D)
    arm_2d_init();
    disp_adapter0_init();

    arm_2d_scene_player_register_before_switching_event_handler(
            &DISP0_ADAPTER,
            before_scene_switching_handler);
    
    arm_2d_scene_player_set_switching_mode( &DISP0_ADAPTER,
                                            ARM_2D_SCENE_SWITCH_MODE_FADE_WHITE);
    arm_2d_scene_player_set_switching_period(&DISP0_ADAPTER, 3000);
    
    arm_2d_scene_player_switch_to_next_scene(&DISP0_ADAPTER);
    
#   if defined(RTE_Acceleration_Arm_2D_Extra_Benchmark)
    arm_2d_run_benchmark();
#else
    //arm_2d_scene_player_switch_to_next_scene(&DISP0_ADAPTER);
#   endif

#endif

    while (true) {
        led_breath();

#if defined(__RTE_ACCELERATION_ARM_2D__) || defined(RTE_Acceleration_Arm_2D)
        disp_adapter0_task();
#endif
    }
    //return 0;
}
