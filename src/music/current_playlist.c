#include "current_playlist.h"
#include "config/osu_path.h"
#include "music/playlist.h"
#include "music/songs.h"
#include "osu/osu_db.h"
#include "osu/osu_file_reading.h"
#include <stdlib.h>
#include <stdio.h>
#include <raylib/raylib.h>

static int nBeatmaps;
static Beatmap *loadedBeatmaps;

static int nCollections;
static Collection *loadedCollections;

static char const ** playlistNames;
static char const ** playlistSongNames;

static Playlist *playlist = NULL;

static int __load_beatmaps(Beatmap **beatmaps);
static int __load_collections(Collection **collections);
static void __create_playlist_for_beatmaps(Collection* collection, Beatmap* beatmaps, int nBeatmaps);

void load_playlists()
{
  nBeatmaps = __load_beatmaps(&loadedBeatmaps);
  if (nBeatmaps == -1) {
    fprintf(stderr, "Failed to load beatmaps\n");
    return;
  }
  
  nCollections = __load_collections(&loadedCollections);
  if (nCollections == -1) {
    fprintf(stderr, "Failed to load collections\n");
    return;
  }

  playlistNames = (char const **)malloc((nCollections+1) * sizeof(char*));
  playlistNames[0] = "All";
  for (int i = 0; i < nCollections; i++) playlistNames[i+1] = loadedCollections[i].name;

  __create_playlist_for_beatmaps(NULL, loadedBeatmaps, nBeatmaps);
}

Playlist* current_playlist()
{
  return playlist;
}

int number_of_playlists()
{
  return nCollections+1;
}

char const ** playlist_names()
{
  return playlistNames;
}

char const ** playlist_song_names()
{
  return playlistSongNames;
}

void set_current_playlist(unsigned int index)
{
  Collection *collection = index == 0
    ? NULL
    : &loadedCollections[index-1];

  __create_playlist_for_beatmaps(collection, loadedBeatmaps, nBeatmaps);
}

void set_playlist_current_song(unsigned int index)
{
  if (index < 0 || index >= playlist->nSongs) return;
  playlist->currentSong = index;
}

void unload_playlists()
{
  free_beatmaps(loadedBeatmaps, nBeatmaps);
  free_collections(loadedCollections, nCollections);
  free_playlist(playlist);
  free(playlistNames);
  free(playlistSongNames);
}

static int __load_beatmaps(Beatmap **beatmaps)
{
  char const *osuDbPath = get_osu_db_path();
  OsuFile osuFile = open_osu_file(osuDbPath);
  if (osuFile.fptr == NULL) {
    perror(TextFormat("Couldn't open osu file: %s", osuDbPath));
    return -1;
  }

  int nBeatmaps;
  *beatmaps = read_beatmaps(&osuFile, &nBeatmaps);

  close_osu_file(&osuFile);

  return nBeatmaps;
}

static int __load_collections(Collection **collections)
{
  char const *osuCollectionsPath = get_osu_collections_path();
  OsuFile osuFile = open_osu_file(osuCollectionsPath);
  if (osuFile.fptr == NULL) {
    perror(TextFormat("Couldn't open osu file: %s", osuCollectionsPath));
    return -1;
  }

  int nCollections;
  *collections = read_collections(&osuFile, &nCollections);

  close_osu_file(&osuFile);

  return nCollections;  
}

static void __create_playlist_for_beatmaps(Collection* collection, Beatmap* beatmaps, int nBeatmaps)
{
  if (playlist != NULL) {
    free_playlist(playlist);
    free(playlistSongNames);
  }

  int nSongs;
  Song *songs = collection == NULL
    ? extract_songs(beatmaps, nBeatmaps, &nSongs)
    : extract_songs_for_collection(collection, beatmaps, nBeatmaps, &nSongs);

  char const *playlistName = collection == NULL
    ? "All"
    : collection->name;
  
  playlist = create_playlist_for_songs(songs, nSongs, playlistName);

  playlistSongNames = (char const**)malloc(nSongs * sizeof(char*));
  for (int i = 0; i < nSongs; i++)
    playlistSongNames[i] = songs[i].songTitle;
}
