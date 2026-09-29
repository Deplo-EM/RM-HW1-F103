/*
 * @Author: Redistuo 15869483306@163.com
 * @Date: 2026-09-29 15:41:23
 * @LastEditors: Redistuo 15869483306@163.com
 * @LastEditTime: 2026-09-29 19:04:27
 * @FilePath: \HW1\Tasks\inc\Tasks.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#ifndef TASKS_H
#define TASKS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>   /* uint32_t 这个类型名来自这里 */


    extern volatile uint32_t tick;


    void TasksInit(void);

#ifdef __cplusplus
}
#endif

#endif /* TASKS_H */
