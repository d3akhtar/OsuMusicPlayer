#include "music/playlist.h"
#include "music/songs.h"
#include "osu/osu_db.h"
#include <osu/osu_file_reading.h>
#include <stdio.h>
#include <stdlib.h>

void PrintBeatmap(Beatmap* beatmap);
void PrintCollection(Collection* collection);
void PrintSong(Song* song);
void PrintPlaylist(Playlist* playlist);

int TestReadingOsuDb(char const *path);
int TestReadingOsuCollections(char const *path);
int TestExtractSongs(Beatmap* beatmaps, int nBeatmaps);
int TestExtractSongsFromCollection(Collection* collection, Beatmap* beatmaps, int nBeatmaps);
int TestCreatePlaylistForSongs(Song* songs, int nSongs);

int main()
{
  printf("Starting tests...\n");
  
  char const * osuDbPath = "./tests/data/osu!.db";
  char const * collectionsPath = "./tests/data/collection.db";

  if (!TestReadingOsuDb(osuDbPath)) {
    fprintf(stderr, "TestReadingOsuDb failed\n");
    exit(-1);
  }

  if (!TestReadingOsuCollections(collectionsPath)) {
    fprintf(stderr, "TestReadingOsuCollections failed\n");
    exit(-1);
  }

  OsuFile file = OpenOsuFile(osuDbPath);
  if (file.fptr == NULL) {
    fprintf(stderr, "Failed to open file: %s\n", osuDbPath);
    exit(-1);
  }

  int nBeatmaps;
  Beatmap* beatmaps = ReadBeatmaps(&file, &nBeatmaps);

  if (!TestExtractSongs(beatmaps, nBeatmaps)) {
    fprintf(stderr, "TestExtractSongs failed\n");
    exit(-1);    
  }

  OsuFile collectionDb = OpenOsuFile(collectionsPath);
  if (file.fptr == NULL) {
    fprintf(stderr, "Failed to open file: %s\n", collectionsPath);
    exit(-1);
  }

  int nCollections;
  Collection* collections = ReadCollections(&collectionDb, &nCollections);

  if (nCollections <= 0) {
    fprintf(stderr, "Failed to read any collections\n");
    exit(-1);
  }

  if (!TestExtractSongsFromCollection(&collections[0], beatmaps, nBeatmaps)) {
    fprintf(stderr, "TestExtractSongsFromCollection failed\n");
    exit(-1);    
  }
}

void PrintBeatmap(Beatmap* beatmap)
{
  printf("[%s] %s - %s => Song location: %s/%s\n", beatmap->mD5Hash, beatmap->artistName, beatmap->songTitle, beatmap->folderName, beatmap->audioFileName);
}

void PrintCollection(Collection* collection)
{
  printf("%s - %d maps\n", collection->name, collection->nBeatmapHashes);
}

void PrintSong(Song* song)
{
  printf("%s => %s\n", song->songInfo, song->audioFilePath);
}

void PrintPlaylist(Playlist* playlist)
{
  printf("Number of songs%d\n", playlist->nSongs);
  for (int i = 0; i < playlist->nSongs; i++)
  {
    printf("\t");
    PrintSong(&playlist->songs[i]);
  }
}

int TestReadingOsuDb(char const *path)
{
  printf("== TestReadingOsuDb(%s) ==\n", path);
  
  OsuFile file = OpenOsuFile(path);
  if (file.fptr == NULL) {
    fprintf(stderr, "Failed to open file: %s\n", path);
    return 0;
  }

  int nBeatmaps;
  Beatmap* beatmaps = ReadBeatmaps(&file, &nBeatmaps);

  printf("Read %d beatmaps, displaying first 20\n", nBeatmaps);

  for (int i = 0; i < 20; i++)
  {
    PrintBeatmap(&beatmaps[i]);
  }

  CloseOsuFile(&file);

  return 1;
}

int TestReadingOsuCollections(char const *path)
{
  printf("== TestReadingOsuCollections(%s) ==\n", path);

  OsuFile file = OpenOsuFile(path);
  if (file.fptr == NULL) {
    fprintf(stderr, "Failed to open file: %s\n", path);
    return 0;
  }

  int nCollections;
  Collection* collections = ReadCollections(&file, &nCollections);

  printf("Read %d collections, displaying first 20\n", nCollections);

  for (int i = 0; i < 20; i++)
  {
    PrintCollection(&collections[i]);
  }

  CloseOsuFile(&file);

  return 1;  
}

int TestExtractSongs(Beatmap* beatmaps, int nBeatmaps)
{
  printf("== TestExtractSongs ==\n");

  int nSongs;
  Song* songs = ExtractSongs(beatmaps, nBeatmaps, &nSongs);

  printf("Extracted %d songs, displaying first 20\n", nSongs);

  for (int i = 0; i < 20; i++)
  {
    printf("%d. ", i+1);
    PrintSong(&songs[i]);
  }

  return 1;
}

int TestExtractSongsFromCollection(Collection* collection, Beatmap* beatmaps, int nBeatmaps)
{
  printf("== TestExtractSongsFromCollection ==\n");  

  int nSongs;
  Song* songs = ExtractSongsForCollection(collection, beatmaps, nBeatmaps, &nSongs);

  printf("Extracted %d songs from collection %s, displaying first 20\n", nSongs, collection->name);

  for (int i = 0; i < 20; i++)
  {
    printf("%d. ", i+1);
    PrintSong(&songs[i]);
  }

  return 1;
}

int TestCreatePlaylistForSongs(Song* songs, int nSongs)
{
  printf("== TestCreatePlaylistForSongs ==\n");

  Playlist playlist = CreatePlaylistForSongs(songs, nSongs);
  PrintPlaylist(&playlist);

  return 1;
}
