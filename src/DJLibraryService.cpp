#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist) {}
/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    library = std::vector<AudioTrack*>();
    //Todo: Implement buildLibrary method
    std::cout << "TODO: Implement DJLibraryService::buildLibrary method\n"<< library_tracks.size() << " tracks to be loaded into library.\n";
    int cnt = 0;
    for (int i = 0; i < library_tracks.size(); i++) {
        SessionConfig::TrackInfo track = library_tracks[i];
        if (track.type == "MP3") {
            MP3Track* mp3_track = new MP3Track(track.title, track.artists, track.duration_seconds, track.bpm, track.extra_param1, track.extra_param2);
            library.push_back(mp3_track);
            std::cout << "MP3: MP3Track created: " << mp3_track->get_bitrate() << " kbps\n"; 
            cnt++;
        }
        else if (library_tracks[i].type == "WAV") {
            WAVTrack* wav_track = new WAVTrack(track.title, track.artists, track.duration_seconds, track.bpm, track.extra_param1, track.extra_param2);
            library.push_back(wav_track);
            std::cout << "WAV: WAVTrack created: " << wav_track->get_sample_rate() << "Hz/" <<wav_track->get_bit_depth() << "bit\n";
            cnt++;
        }
    }
    std::cout << "[INFO] Track library built: "<< cnt << " tracks loaded\n";
}

/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 * 
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    return playlist.find_track(track_title);
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, const std::vector<int>& track_indices) {

    std::cout << "[INFO] Loading playlist: " << playlist_name << std::endl;
    playlist = Playlist(playlist_name);
    int cnt =0;
    for(int idx : track_indices){
        if(idx < 1 || idx > library.size()){
            std::cerr << "[WARNING] Invalid track index: " << idx << std::endl;
            continue;
        }
        else{
            PointerWrapper<AudioTrack> track_wrapper = library[idx-1]->clone();
            if(!track_wrapper){
                std::cerr << "[WARNING] the Pointer Wrapper is null pointer: " << std::endl;
                continue;
            }
            else{
                AudioTrack* track = track_wrapper.release();
                track->load();
                track->analyze_beatgrid();
                playlist.add_track(track);
                std::cout << "Added '" << track->get_title() << "' to playlist '" << playlist_name << "'\n";
                cnt++;
            }
        } 
    }
    std::cout<<"[INFO] Playlist loaded: "<< playlist_name << " (" << cnt << " tracks)\n";
    // For now, add a placeholder to fix the linker error
    //(void)playlist_name;  // Suppress unused parameter warning
    //(void)track_indices;  // Suppress unused parameter warning
}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    std::vector<std::string> tracks_titles = std::vector<std::string>();
    for(AudioTrack* track : library){
        if(track != nullptr){
            tracks_titles.push_back(track->get_title());
        }
    }
    return tracks_titles;
}
