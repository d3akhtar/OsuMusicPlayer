#include "osu_path_dialog.h"
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
static char * osuPath;
static char const * defaultErrorMessage = "Invalid osu! path";
static char const * errorMessage = "";

static bool __validate_osu_path(char const * path);
static void __save_osu_path(char const *path);

void init_osu_path()
{
  FILE* fptr = fopen(osuPathStorageFile, "rb");
  if (fptr == NULL) {
    printf("Osu path not saved at path %s yet\n", osuPathStorageFile);
    showSelectOsuPathDialog = true;
    osuPath = OSU_PATH_DEFAULT_VALUE;
    return;
  }

  char buf[1024];
  size_t len = 0;

  len = fread(buf, sizeof(char), 1024, fptr);

  if (ferror(fptr)) {
    fprintf(stderr, "Error while reading path file: %s\n", osuPathStorageFile);
    fclose(fptr);
    return;
  }

  osuPath = (char*)malloc(len);
  memcpy(osuPath, buf, len);

  fclose(fptr);
}

char const * get_osu_path()
{
  if (osuPath == NULL || strcmp(osuPath, OSU_PATH_DEFAULT_VALUE) == 0) return NULL;
  else return osuPath;
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
        errorMessage = !__validate_osu_path(osuPath)
          ? defaultErrorMessage
          : "";
    }

    GuiSetState(strlen(errorMessage) > 0 || strcmp(osuPath, OSU_PATH_DEFAULT_VALUE) == 0 ? STATE_DISABLED : STATE_NORMAL);
    if (GuiButton((Rectangle){150, 370, 980, 50}, "Confirm")) {
        if (__validate_osu_path(osuPath)) {
          __save_osu_path(osuPath);
          showSelectOsuPathDialog = false;
        }
    }
    GuiSetState(STATE_NORMAL);    
  }
}

static bool __validate_osu_path(char const * path)
{
  DIR *dir = opendir(path);

  if (dir == NULL) {
    fprintf(stderr, "Error while opening directory: %s\n", path);
    return false;
  }
  
  bool containsSongsFolder = false, containsOsuDbFile = false;
  struct dirent *ent;

  while ((ent = readdir(dir)) != NULL)
  {
    if (ent->d_type == DT_DIR && strcmp("Songs", ent->d_name) == 0) containsSongsFolder = true;
    else if (strcmp("osu!.db", ent->d_name) == 0) containsOsuDbFile = true;

    if (containsSongsFolder && containsOsuDbFile) break;
  }

  closedir(dir);
  
  return containsSongsFolder && containsOsuDbFile;
}

static void __save_osu_path(char const *path)
{
  FILE* fptr = fopen(osuPathStorageFile, "wb");

  if (fptr == NULL) {
    fprintf(stderr, "Error while opening path file %s for write\n", path);
    return;
  }

  size_t len = strlen(path);
  size_t wrote = fwrite(path, sizeof(char), len, fptr);
  if (wrote != len) {
    fprintf(stderr, "Error while writing %zu bytes to file (wrote %zu bytes instead)\n", len, wrote);
    fclose(fptr);
    if (remove(path) != 0) fprintf(stderr, "Failed to remove file: %s after failed write\n", path); 
    return;
  }

  fclose(fptr);
  return;
}
