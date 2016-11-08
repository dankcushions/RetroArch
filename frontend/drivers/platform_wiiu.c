/* RetroArch - A frontend for libretro.
 *  Copyright (C) 2014-2016 - Ali Bouhlel
 *
 * RetroArch is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Found-
 * ation, either version 3 of the License, or (at your option) any later version.
 *
 * RetroArch is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with RetroArch.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <boolean.h>

#include <file/file_path.h>

#include <lists/file_list.h>

#include "../frontend_driver.h"
#include "../frontend.h"
#include "../../configuration.h"
#include "../../verbosity.h"
#include "../../defaults.h"
#include "../../paths.h"
#include "retroarch.h"
#include "file_path_special.h"
#include "audio/audio_driver.h"


#include "tasks/tasks_internal.h"
#include "runloop.h"
#include <nsysnet/socket.h>
#include "fs/fs_utils.h"
#include "fs/sd_fat_devoptab.h"
#include "system/dynamic.h"
#include "system/memory.h"
#include "system/exception_handler.h"
#include "system/exception.h"
#include <sys/iosupport.h>

#include <coreinit/screen.h>
#include <vpad/input.h>

#include "wiiu_dbg.h"

#ifdef HAVE_MENU
#include "../../menu/menu_driver.h"
#endif

static enum frontend_fork wiiu_fork_mode = FRONTEND_FORK_NONE;
static const char* elf_path_cst = "sd:/retroarch/retroarch.elf";

static void frontend_wiiu_get_environment_settings(int *argc, char *argv[],
      void *args, void *params_data)
{
   (void)args;
   DEBUG_LINE();

   fill_pathname_basedir(g_defaults.dir.port, elf_path_cst, sizeof(g_defaults.dir.port));
   DEBUG_LINE();
   RARCH_LOG("port dir: [%s]\n", g_defaults.dir.port);

   fill_pathname_join(g_defaults.dir.core_assets, g_defaults.dir.port,
         "downloads", sizeof(g_defaults.dir.core_assets));
   fill_pathname_join(g_defaults.dir.assets, g_defaults.dir.port,
         "media", sizeof(g_defaults.dir.assets));
   fill_pathname_join(g_defaults.dir.core, g_defaults.dir.port,
         "cores", sizeof(g_defaults.dir.core));
   fill_pathname_join(g_defaults.dir.core_info, g_defaults.dir.core,
         "info", sizeof(g_defaults.dir.core_info));
   fill_pathname_join(g_defaults.dir.savestate, g_defaults.dir.core,
         "savestates", sizeof(g_defaults.dir.savestate));
   fill_pathname_join(g_defaults.dir.sram, g_defaults.dir.core,
         "savefiles", sizeof(g_defaults.dir.sram));
   fill_pathname_join(g_defaults.dir.system, g_defaults.dir.core,
         "system", sizeof(g_defaults.dir.system));
   fill_pathname_join(g_defaults.dir.playlist, g_defaults.dir.core,
         "playlists", sizeof(g_defaults.dir.playlist));
   fill_pathname_join(g_defaults.dir.menu_config, g_defaults.dir.port,
         "config", sizeof(g_defaults.dir.menu_config));
   fill_pathname_join(g_defaults.dir.remap, g_defaults.dir.port,
         "config/remaps", sizeof(g_defaults.dir.remap));
   fill_pathname_join(g_defaults.dir.video_filter, g_defaults.dir.port,
         "filters", sizeof(g_defaults.dir.remap));
   fill_pathname_join(g_defaults.dir.database, g_defaults.dir.port,
         "database/rdb", sizeof(g_defaults.dir.database));
   fill_pathname_join(g_defaults.dir.cursor, g_defaults.dir.port,
         "database/cursors", sizeof(g_defaults.dir.cursor));
   fill_pathname_join(g_defaults.path.config, g_defaults.dir.port,
         file_path_str(FILE_PATH_MAIN_CONFIG), sizeof(g_defaults.path.config));
}

static void frontend_wiiu_deinit(void *data)
{
   (void)data;
}

static void frontend_wiiu_shutdown(bool unused)
{
   (void)unused;
}

static void frontend_wiiu_init(void *data)
{
   (void)data;
   DEBUG_LINE();
   verbosity_enable();
   DEBUG_LINE();
}


static int frontend_wiiu_get_rating(void)
{
   return 10;
}

enum frontend_architecture frontend_wiiu_get_architecture(void)
{
   return FRONTEND_ARCH_PPC;
}

static int frontend_wiiu_parse_drive_list(void *data)
{
   file_list_t *list = (file_list_t*)data;

   if (!list)
      return -1;

   menu_entries_append_enum(list,
         "sd:/", "", MSG_UNKNOWN, FILE_TYPE_DIRECTORY, 0, 0);

   return 0;
}

frontend_ctx_driver_t frontend_ctx_wiiu = {
   frontend_wiiu_get_environment_settings,
   frontend_wiiu_init,
   frontend_wiiu_deinit,
   NULL,                         /* exitspawn */
   NULL,                         /* process_args */
   NULL,                         /* exec */
   NULL,                         /* set_fork */
   frontend_wiiu_shutdown,
   NULL,                         /* get_name */
   NULL,                         /* get_os */
   frontend_wiiu_get_rating,
   NULL,                         /* load_content */
   frontend_wiiu_get_architecture,
   NULL,                         /* get_powerstate */
   frontend_wiiu_parse_drive_list,
   NULL,                         /* get_mem_total */
   NULL,                         /* get_mem_free */
   NULL,                         /* install_signal_handler */
   NULL,                         /* get_signal_handler_state */
   NULL,                         /* set_signal_handler_state */
   NULL,                         /* destroy_signal_handler_state */
   NULL,                         /* attach_console */
   NULL,                         /* detach_console */
   "wiiu",
};

static int log_socket = -1;
static volatile int log_lock = 0;

void log_init(const char * ipString)
{
	log_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (log_socket < 0)
		return;

	struct sockaddr_in connect_addr;
	memset(&connect_addr, 0, sizeof(connect_addr));
	connect_addr.sin_family = AF_INET;
	connect_addr.sin_port = 4405;
	inet_aton(ipString, &connect_addr.sin_addr);

	if(connect(log_socket, (struct sockaddr*)&connect_addr, sizeof(connect_addr)) < 0)
	{
	    socketclose(log_socket);
	    log_socket = -1;
	}
}

void log_deinit(void)
{
    if(log_socket >= 0)
    {
        socketclose(log_socket);
        log_socket = -1;
    }
}
static int log_write(struct _reent *r, int fd, const char *ptr, size_t len)
{
   if(log_socket < 0)
       return len;

   while(log_lock)
      OSSleepTicks(((248625000/4)) / 1000);
   log_lock = 1;

   int ret;
   while (len > 0) {
       int block = len < 1400 ? len : 1400; // take max 1400 bytes per UDP packet
       ret = send(log_socket, ptr, block, 0);
       if(ret < 0)
           break;

       len -= ret;
       ptr += ret;
   }

   log_lock = 0;

   return len;
}
void net_print(const char* str)
{
   log_write(NULL, 0, str, strlen(str));
}

void net_print_exp(const char* str)
{
   send(log_socket, str, strlen(str), 0);
}

static devoptab_t dotab_stdout = {
   "stdout_net", // device name
   0,            // size of file structure
   NULL,         // device open
   NULL,         // device close
   log_write,    // device write
   NULL,
};

int __entry_menu(int argc, char **argv)
{
   InitFunctionPointers();
#if 1
   setup_os_exceptions();
#else
   InstallExceptionHandler();
#endif
   socket_lib_init();
#if 0
   log_init("10.42.0.1");
#endif
   devoptab_list[STD_OUT] = &dotab_stdout;
   devoptab_list[STD_ERR] = &dotab_stdout;
   memoryInitialize();
   mount_sd_fat("sd");
   VPADInit();

   verbosity_enable();
   DEBUG_VAR(argc);
   DEBUG_STR(argv[0]);
   DEBUG_STR(argv[1]);
#if 0
   int argc_ = 2;
//   char* argv_[] = {"sd:/retroarch/retroarch.elf", "sd:/rom.nes", NULL};
   char* argv_[] = {"sd:/retroarch/retroarch.elf", "sd:/rom.sfc", NULL};

   rarch_main(argc_, argv_, NULL);
#else
   rarch_main(argc, argv, NULL);
#endif
//   int frames = 0;
   do
   {
      unsigned sleep_ms = 0;
      int ret = runloop_iterate(&sleep_ms);

      if (ret == 1 && sleep_ms > 0)
       retro_sleep(sleep_ms);
      task_queue_ctl(TASK_QUEUE_CTL_CHECK, NULL);
      if (ret == -1)
       break;

   }while(1);
//   }while(frames++ < 300);

   main_exit(NULL);
   unmount_sd_fat("sd");
   memoryRelease();
   fflush(stdout);
   fflush(stderr);
   log_deinit();

   return 0;
}

unsigned long _times_r(struct _reent *r, struct tms *tmsbuf)
{
   return 0;
}
