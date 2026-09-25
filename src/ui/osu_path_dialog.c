#include "osu_path_dialog.h"
#include "config/osu_path.h"
#include "raylib/raylib.h"
#include "raylib/raygui.h"
#include "tinyfiledialogs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NOGDI
#define NOUSER
#include <dirent.h>
#undef NOGDI
#undef NOUSER

#define OSU_PATH_DEFAULT_VALUE "{SELECT OSU! PATH}"

static char const * osuPathStorageFile = "./path.db";

static bool showSelectOsuPathDialog = false;
static char const * osuPath = NULL;
static char const * defaultErrorMessage = "Invalid osu! path";
static char const * errorMessage = "";

void init_osu_path_dialog()
{
  init_osu_path();
  const char *configOsuPath = get_osu_path();
  if (configOsuPath == NULL) {
    showSelectOsuPathDialog = true;
    osuPath = OSU_PATH_DEFAULT_VALUE;
  }
  else osuPath = configOsuPath;
}

void gui_draw_osu_path_dialog()
{
  if (showSelectOsuPathDialog) {
    GuiPanel((Rectangle) {140, 230, 1000, 200}, "Select osu! Path");
    DrawText(errorMessage, 540, 263, 18, RED);
    DrawText("Path:", 180, 290, 60, BLACK);
    DrawRectangleLines(350, 290, 650, 60, BLACK);
    DrawText(osuPath, 360, 310, 20, (Color){133, 90, 129, 255});

    if (GuiButton((Rectangle) {1010, 290, 90, 60}, "Browse")) {
        osuPath = (char*)tinyfd_selectFolderDialog("Select osu! location", "");
        errorMessage = !validate_osu_path(osuPath)
          ? defaultErrorMessage
          : "";
    }

    GuiSetState(strlen(errorMessage) > 0 || strcmp(osuPath, OSU_PATH_DEFAULT_VALUE) == 0 ? STATE_DISABLED : STATE_NORMAL);
    if (GuiButton((Rectangle){150, 370, 980, 50}, "Confirm")) {
        save_osu_path(osuPath);
        showSelectOsuPathDialog = false;
    }
    GuiSetState(STATE_NORMAL);    
  }
}
