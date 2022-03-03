/**
  ******************************************************************************
  * @file          fx_bl706_lx_xip_driver.h
  * @brief         filex ram driver
  * @author        Egahp
  *                2687434412@qq.com
  * @version       1.0
  * @date          2022.02.22
  ******************************************************************************
  * @attention
  * 
  * <h2><center>&copy; Copyright 2021 Egahp.
  * All rights reserved.</center></h2>
  * 
  * @htmlonly 
  * <span style='font-weight: bold'>History</span> 
  * @endhtmlonly
  * 版本|作者|时间|描述
  * ----|----|----|----
  * 1.0|Egahp|2022.02.22|创建文件
  ******************************************************************************
  */

#ifndef __FX_BL706_FLASH_DRIVER_H__
#define __FX_BL706_FLASH_DRIVER_H__

#ifdef __cplusplus
extern "C" {
#endif

/* include -------------------------------------------------------------------*/
#include "fx_api.h"
#include "lx_bl706_xip_driver.h"

/* marco ---------------------------------------------------------------------*/


/* typedef -------------------------------------------------------------------*/

/* declare -------------------------------------------------------------------*/

/* variable ------------------------------------------------------------------*/
extern LX_NOR_FLASH xip_flash;
/* code ----------------------------------------------------------------------*/
VOID fx_bl706_lx_xip_driver(FX_MEDIA *media_ptr);

#ifdef __cplusplus
}
#endif
#endif 
/************************ (C) COPYRIGHT 2021 Egahp *****END OF FILE*************/
