/**
  ******************************************************************************
  * @file          fx_bl706_lx_xip_driver.c
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


/* include -------------------------------------------------------------------*/
#include "fx_bl706_lx_xip_driver.h"
#include "bflb_platform.h"
/* marco ---------------------------------------------------------------------*/

/* typedef -------------------------------------------------------------------*/

/* declare -------------------------------------------------------------------*/

/* variable ------------------------------------------------------------------*/
LX_NOR_FLASH xip_flash;
/* code ----------------------------------------------------------------------*/

VOID fx_bl706_lx_xip_driver(FX_MEDIA *media_ptr)
{
    UCHAR       *source_buffer;
    UCHAR       *destination_buffer;
    ULONG       logical_sector;
    ULONG       i;
    UINT        status;

    /* There are several useful/important pieces of information contained in the media
        structure, some of which are supplied by FileX and others are for the driver to
        setup. The following is a summary of the necessary FX_MEDIA structure members:

            FX_MEDIA Member                              Meaning
                                        
        fx_media_driver_request             FileX request type. Valid requests from FileX are 
                                            as follows:

                                                    FX_DRIVER_READ
                                                    FX_DRIVER_WRITE                 
                                                    FX_DRIVER_FLUSH
                                                    FX_DRIVER_ABORT
                                                    FX_DRIVER_INIT
                                                    FX_DRIVER_BOOT_READ
                                                    FX_DRIVER_RELEASE_SECTORS
                                                    FX_DRIVER_BOOT_WRITE
                                                    FX_DRIVER_UNINIT

        fx_media_driver_status              This value is RETURNED by the driver. If the 
                                            operation is successful, this field should be
                                            set to FX_SUCCESS for before returning. Otherwise,
                                            if an error occurred, this field should be set
                                            to FX_IO_ERROR. 

        fx_media_driver_buffer              Pointer to buffer to read or write sector data.
                                            This is supplied by FileX.

        fx_media_driver_logical_sector      Logical sector FileX is requesting.

        fx_media_driver_sectors             Number of sectors FileX is requesting.


        The following is a summary of the optional FX_MEDIA structure members:

            FX_MEDIA Member                              Meaning
                                        
        fx_media_driver_info                Pointer to any additional information or memory.
                                            This is optional for the driver use and is setup
                                            from the fx_media_open call. The RAM disk uses
                                            this pointer for the RAM disk memory itself.

        fx_media_driver_write_protect       The DRIVER sets this to FX_TRUE when media is write 
                                            protected. This is typically done in initialization, 
                                            but can be done anytime.

        fx_media_driver_free_sector_update  The DRIVER sets this to FX_TRUE when it needs to 
                                            know when clusters are released. This is important
                                            for FLASH wear-leveling drivers.

        fx_media_driver_system_write        FileX sets this flag to FX_TRUE if the sector being
                                            written is a system sector, e.g., a boot, FAT, or 
                                            directory sector. The driver may choose to use this
                                            to initiate error recovery logic for greater fault
                                            tolerance.

        fx_media_driver_data_sector_read    FileX sets this flag to FX_TRUE if the sector(s) being
                                            read are file data sectors, i.e., NOT system sectors.

        fx_media_driver_sector_type         FileX sets this variable to the specific type of 
                                            sector being read or written. The following sector
                                            types are identified:

                                                    FX_UNKNOWN_SECTOR 
                                                    FX_BOOT_SECTOR
                                                    FX_FAT_SECTOR
                                                    FX_DIRECTORY_SECTOR
                                                    FX_DATA_SECTOR  
    */

    /* Process the driver request specified in the media control block.  */
    switch(media_ptr -> fx_media_driver_request)
    {

        case FX_DRIVER_READ:
        {

            /* Setup the destination buffer and logical sector.  */
            logical_sector =      media_ptr -> fx_media_driver_logical_sector;
            destination_buffer =  (UCHAR *) media_ptr -> fx_media_driver_buffer;

            /* Loop to read sectors from flash.  */
            for (i = 0; i < media_ptr -> fx_media_driver_sectors; i++)
            {
            
                /* Read a sector from NOR flash.  */
                status =  lx_nor_flash_sector_read(&xip_flash, logical_sector, destination_buffer);

                /* Determine if the read was successful.  */
                if (status != LX_SUCCESS)
                {
                
                    /* Return an I/O error to FileX.  */
                    media_ptr -> fx_media_driver_status =  FX_IO_ERROR;
                    
                    return;
                } 

                /* Move to the next entries.  */
                logical_sector++;
                destination_buffer =  destination_buffer + 512;
            }

            /* Successful driver request.  */
            media_ptr -> fx_media_driver_status =  FX_SUCCESS;
            break;
        }
        
        case FX_DRIVER_WRITE:
        {

            /* Setup the source buffer and logical sector.  */
            logical_sector =      media_ptr -> fx_media_driver_logical_sector;
            source_buffer =       (UCHAR *) media_ptr -> fx_media_driver_buffer;

            /* Loop to write sectors to flash.  */
            for (i = 0; i < media_ptr -> fx_media_driver_sectors; i++)
            {
            
                /* Write a sector to NOR flash.  */
                status =  lx_nor_flash_sector_write(&xip_flash, logical_sector, source_buffer);

                /* Determine if the write was successful.  */
                if (status != LX_SUCCESS)
                {
                
                    /* Return an I/O error to FileX.  */
                    media_ptr -> fx_media_driver_status =  FX_IO_ERROR;
                    
                    return;
                } 

                /* Move to the next entries.  */
                logical_sector++;
                source_buffer =  source_buffer + 512;
            }

            /* Successful driver request.  */
            media_ptr -> fx_media_driver_status =  FX_SUCCESS;
            break;
        }

        case FX_DRIVER_RELEASE_SECTORS:
        {
        
            /* Setup the logical sector.  */
            logical_sector =  media_ptr -> fx_media_driver_logical_sector;

            /* Release sectors.  */
            for (i = 0; i < media_ptr -> fx_media_driver_sectors; i++)
            {
            
                /* Release NOR flash sector.  */
                status =  lx_nor_flash_sector_release(&xip_flash, logical_sector);

                /* Determine if the sector release was successful.  */
                if (status != LX_SUCCESS)
                {
                
                    /* Return an I/O error to FileX.  */
                    media_ptr -> fx_media_driver_status =  FX_IO_ERROR;
                    
                    return;
                } 

                /* Move to the next entries.  */
                logical_sector++;
            }

            /* Successful driver request.  */
            media_ptr -> fx_media_driver_status =  FX_SUCCESS;
            break;
        }

        case FX_DRIVER_FLUSH:
        {

            /* Return driver success.  */
            media_ptr -> fx_media_driver_status =  FX_SUCCESS;
            break;
        }

        case FX_DRIVER_ABORT:
        {

            /* Return driver success.  */
            media_ptr -> fx_media_driver_status =  FX_SUCCESS;
            break;
        }

        case FX_DRIVER_INIT:
        {

            /* FLASH drivers are responsible for setting several fields in the 
                media structure, as follows:

                    media_ptr -> fx_media_driver_free_sector_update
                    media_ptr -> fx_media_driver_write_protect

                The fx_media_driver_free_sector_update flag is used to instruct
                FileX to inform the driver whenever sectors are not being used.
                This is especially useful for FLASH managers so they don't have 
                maintain mapping for sectors no longer in use.

                The fx_media_driver_write_protect flag can be set anytime by the
                driver to indicate the media is not writable.  Write attempts made
                when this flag is set are returned as errors.  */

            /* Perform basic initialization here... since the boot record is going
                to be read subsequently and again for volume name requests.  */

            /* With flash wear leveling, FileX should tell wear leveling when sectors
                are no longer in use.  */
            media_ptr -> fx_media_driver_free_sector_update =  FX_TRUE;

            /* Open the NOR flash simulation.  */
            status =  lx_nor_flash_open(&xip_flash, "xip flash", lx_bl706_xip_driver_initialize);

            /* Determine if the flash open was successful.  */
            if (status != LX_SUCCESS)
            {
                /* Return an I/O error to FileX.  */
                LOG_E("filex driver init, levelx open error [%d]\r\n", status);
                media_ptr -> fx_media_driver_status =  FX_IO_ERROR;

                return;
            } 

            /* Successful driver request.  */
            media_ptr -> fx_media_driver_status =  FX_SUCCESS;
            break;
        }

        case FX_DRIVER_UNINIT:
        {

            /* There is nothing to do in this case for the RAM driver.  For actual
               devices some shutdown processing may be necessary.  */

            /* Close the NOR flash simulation.  */
            status =  lx_nor_flash_close(&xip_flash);

            /* Determine if the flash close was successful.  */
            if (status != LX_SUCCESS)
            {
                
                /* Return an I/O error to FileX.  */
                LOG_E("filex driver uninit, levelx close error [%d]\r\n", status);
                media_ptr -> fx_media_driver_status =  FX_IO_ERROR;
                    
                return;
            } 

            /* Successful driver request.  */
            media_ptr -> fx_media_driver_status =  FX_SUCCESS;
            break;
        }
        
        case FX_DRIVER_BOOT_READ:
        {

            /* Read the boot record and return to the caller.  */

            /* Setup the destination buffer.  */
            destination_buffer =  (UCHAR *) media_ptr -> fx_media_driver_buffer;

            /* Read boot sector from NOR flash.  */
            status =  lx_nor_flash_sector_read(&xip_flash, 0, destination_buffer);

            /* For NOR driver, determine if the boot record is valid.  */
            if ((destination_buffer[0] != (UCHAR) 0xEB) ||
                (destination_buffer[1] != (UCHAR) 0x34) ||
                (destination_buffer[2] != (UCHAR) 0x90))
            {

                /* Invalid boot record, return an error!  */
                media_ptr -> fx_media_driver_status =  FX_MEDIA_INVALID;
                return;
            }

            /* Determine if the boot read was successful.  */
            if (status != LX_SUCCESS)
            {
                
                /* Return an I/O error to FileX.  */
                media_ptr -> fx_media_driver_status =  FX_IO_ERROR;
                    
                return;
            } 

            /* Successful driver request.  */
            media_ptr -> fx_media_driver_status =  FX_SUCCESS;
            break;
        }

        case FX_DRIVER_BOOT_WRITE:
        {

            /* Write the boot record and return to the caller.  */

            /* Setup the source buffer.  */
            source_buffer =       (UCHAR *) media_ptr -> fx_media_driver_buffer;

            /* Write boot sector to NOR flash.  */
            status =  lx_nor_flash_sector_write(&xip_flash, 0, source_buffer);

            /* Determine if the boot write was successful.  */
            if (status != LX_SUCCESS)
            {
                
                /* Return an I/O error to FileX.  */
                media_ptr -> fx_media_driver_status =  FX_IO_ERROR;
                    
                return;
            } 

            /* Successful driver request.  */
            media_ptr -> fx_media_driver_status =  FX_SUCCESS;
            break ;
        }

        default:
        {

            /* Invalid driver request.  */
            LOG_E("filex driver invalid request\r\n");
            media_ptr -> fx_media_driver_status =  FX_IO_ERROR;
            break;
        }
    }
}

/************************ (C) COPYRIGHT 2021 Egahp *****END OF FILE*************/
