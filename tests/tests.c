#include "music/playlist.h"
#include "music/songs.h"
#include "osu/osu_db.h"
#include "utils/path.h"
#include "utils/string.h"
#include <osu/osu_file_reading.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_beatmap(Beatmap* beatmap);
void print_collection(Collection* collection);
void print_song(Song* song);
void print_playlist(Playlist* playlist);

int test_split_into_lines(char const * str);
int test_join_paths(char const* a, char const* b);
int test_reading_osu_db(char const *path);
int test_reading_osu_collections(char const *path);
int test_extract_songs(Beatmap* beatmaps, int nBeatmaps);
int test_extract_songs_from_collections(Collection* collection, Beatmap* beatmaps, int nBeatmaps);
int test_create_playlist_for_songs(Song* songs, int nSongs);
int test_extract_bg_file_name(char const * path);
int test_create_list_view_text_for_playlist(Playlist* playlist);

int main()
{
  printf("Starting tests...\n");

  if (!test_split_into_lines("abc\ndef\nhij\nabc\ndeffefef\niejroiwjrw")) {
    fprintf(stderr, "test_split_into_lines failed\n");
    exit(-1);   
  }

  if (!test_join_paths("directory", "path.txt")) {
    fprintf(stderr, "test_join_paths failed\n");
    exit(-1);   
  }

  if (!test_join_paths("directory/innerDir", "path.txt")) {
    fprintf(stderr, "test_join_paths failed\n");
    exit(-1);   
  }

  if (!test_join_paths("directory/innerDir/", "path.txt")) {
    fprintf(stderr, "test_join_paths failed\n");
    exit(-1);   
  }
  
  char const * osuDbPath = "./tests/data/osu!.db";
  char const * collectionsPath = "./tests/data/collection.db";
  char const * beatmapInfoPath = "./tests/data/testBeatmapInfoFile.osu";

  if (!test_reading_osu_db(osuDbPath)) {
    fprintf(stderr, "test_reading_osu_db failed\n");
    exit(-1);
  }

  if (!test_reading_osu_collections(collectionsPath)) {
    fprintf(stderr, "test_reading_osu_collections failed\n");
    exit(-1);
  }

  OsuFile file = open_osu_file(osuDbPath);
  if (file.fptr == NULL) {
    fprintf(stderr, "Failed to open file: %s\n", osuDbPath);
    exit(-1);
  }

  int nBeatmaps;
  Beatmap* beatmaps = read_beatmaps(&file, &nBeatmaps);

  if (!test_extract_songs(beatmaps, nBeatmaps)) {
    fprintf(stderr, "test_extract_songs failed\n");
    exit(-1);    
  }

  OsuFile collectionDb = open_osu_file(collectionsPath);
  if (file.fptr == NULL) {
    fprintf(stderr, "Failed to open file: %s\n", collectionsPath);
    exit(-1);
  }

  int nCollections;
  Collection* collections = read_collections(&collectionDb, &nCollections);

  if (nCollections <= 0) {
    fprintf(stderr, "Failed to read any collections\n");
    exit(-1);
  }

  if (!test_extract_songs_from_collections(&collections[0], beatmaps, nBeatmaps)) {
    fprintf(stderr, "test_extract_songs_from_collection failed\n");
    exit(-1);    
  }

  int nSongs;
  Song *songs = extract_songs_for_collection(collections, beatmaps, nBeatmaps, &nSongs);

  if (!test_create_playlist_for_songs(songs, nSongs)) {
    fprintf(stderr, "test_create_playlist_for_songs failed\n");
    exit(-1);    
  }

  if (!test_extract_bg_file_name(beatmapInfoPath)) {
    fprintf(stderr, "test_extract_background_file_name failed\n");
    exit(-1);    
  }

  Playlist playlist = create_playlist_for_songs(songs, nSongs, collections[0].name);
  if (!test_create_list_view_text_for_playlist(&playlist)) {
    fprintf(stderr, "test_create_list_view_text_for_playlist failed\n");
    exit(-1);    
  }
}

void print_beatmap(Beatmap* beatmap)
{
  printf("[%s] %s - %s => Song location: %s/%s\n", beatmap->mD5Hash, beatmap->artistName, beatmap->songTitle, beatmap->folderName, beatmap->audioFileName);
}

void print_collection(Collection* collection)
{
  printf("%s - %d maps\n", collection->name, collection->nBeatmapHashes);
}

void print_song(Song* song)
{
  printf("%s - %s (%s)\n", song->artistName, song->songTitle, song->audioFilePath);
}

void print_playlist(Playlist* playlist)
{
  printf("Name: %s, #songs: %d\n", playlist->name, playlist->nSongs);
  for (int i = 0; i < playlist->nSongs; i++)
  {
    printf("\t");
    print_song(&playlist->songs[i]);
  }
}

int test_split_into_lines(char const * str)
{
  printf("== test_split_into_lines(%s) ==\n", str);

  int nLines;
  char ** lines = split_into_lines(str, strlen(str), &nLines);

  printf("Got %d lines:\n", nLines);

  for (int i = 0; i < nLines; i++)
  {
    printf("(%d): %s\n", i+1, lines[i]);
  }

  return 1;
}

int test_join_paths(char const* a, char const* b)
{
  printf("== test_join_paths(%s, %s) ==\n", a, b);

  char *joined = join_paths(a, b);

  printf("Joined path: %s\n", joined);

  return 1;
}

int test_reading_osu_db(char const *path)
{
  printf("== test_reading_osu_db(%s) ==\n", path);
  
  OsuFile file = open_osu_file(path);
  if (file.fptr == NULL) {
    fprintf(stderr, "Failed to open file: %s\n", path);
    return 0;
  }

  int nBeatmaps;
  Beatmap* beatmaps = read_beatmaps(&file, &nBeatmaps);

  printf("Read %d beatmaps, displaying first 20\n", nBeatmaps);

  for (int i = 0; i < 20; i++)
  {
    print_beatmap(&beatmaps[i]);
  }

  close_osu_file(&file);

  return 1;
}

int test_reading_osu_collections(char const *path)
{
  printf("== test_reading_osu_collections(%s) ==\n", path);

  OsuFile file = open_osu_file(path);
  if (file.fptr == NULL) {
    fprintf(stderr, "Failed to open file: %s\n", path);
    return 0;
  }

  int nCollections;
  Collection* collections = read_collections(&file, &nCollections);

  printf("Read %d collections, displaying first 20\n", nCollections);

  for (int i = 0; i < 20; i++)
  {
    print_collection(&collections[i]);
  }

  close_osu_file(&file);

  return 1;  
}

int test_extract_songs(Beatmap* beatmaps, int nBeatmaps)
{
  printf("== test_extract_songs ==\n");

  int nSongs;
  Song* songs = extract_songs(beatmaps, nBeatmaps, &nSongs);

  printf("Extracted %d songs, displaying first 20\n", nSongs);

  for (int i = 0; i < 20; i++)
  {
    printf("%d. ", i+1);
    print_song(&songs[i]);
  }

  return 1;
}

int test_extract_songs_from_collections(Collection* collection, Beatmap* beatmaps, int nBeatmaps)
{
  printf("== test_extract_songs_from_collection ==\n");  

  int nSongs;
  Song* songs = extract_songs_for_collection(collection, beatmaps, nBeatmaps, &nSongs);

  printf("Extracted %d songs from collection %s, displaying first 20\n", nSongs, collection->name);

  for (int i = 0; i < 20; i++)
  {
    printf("%d. ", i+1);
    print_song(&songs[i]);
  }

  return 1;
}

int test_create_playlist_for_songs(Song* songs, int nSongs)
{
  printf("== test_create_playlist_for_songs ==\n");

  Playlist playlist = create_playlist_for_songs(songs, nSongs, "Playlist name");
  print_playlist(&playlist);

  return 1;
}

int test_extract_bg_file_name(char const * path)
{
  printf("== test_extract_background_file_name(%s) ==\n", path);

  char const * backgroundFileName = extract_bg_file_name(path);

  if (backgroundFileName == NULL) {
    fprintf(stderr, "Failed to get background file name\n");
    return 0;
  }

  printf("Retrieved background file name: %s\n", backgroundFileName);

  return 1;
}

int test_create_list_view_text_for_playlist(Playlist* playlist)
{  
  printf("== test_create_list_view_text_for_playlist ==\n");

  char const * listViewText = create_list_view_text_for_playlist(playlist);

  printf("List view text: %s\n", listViewText);

  free((char*)listViewText);

  return 1;
}
