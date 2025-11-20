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
    library = std::vector<AudioTrack*>(library_tracks.size());
    //Todo: Implement buildLibrary method
    std::cout << "TODO: Implement DJLibraryService::buildLibrary method\n"<< library_tracks.size() << " tracks to be loaded into library.\n";
    int cnt = 0;
    for (int i = 0; i < library_tracks.size(); i++) {
        SessionConfig::TrackInfo track = library_tracks[i];
        if (track.type == "MP3") {
            MP3Track* mp3_track = new MP3Track(track.title, track.artists, track.duration_seconds, track.bpm, track.extra_param1, track.extra_param2);
            library[i] = mp3_track;
            std::cout << "MP3: MP3Track created: " << mp3_track->get_bitrate() << " kbps\n"; 
            cnt++;
        }
        else if (library_tracks[i].type == "WAV") {
            WAVTrack* wav_track = new WAVTrack(track.title, track.artists, track.duration_seconds, track.bpm, track.extra_param1, track.extra_param2);
            library[i] = wav_track;
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
    // Your implementation here
    return nullptr; // Placeholder
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
    // Your implementation here
    // For now, add a placeholder to fix the linker error
    (void)playlist_name;  // Suppress unused parameter warning
    (void)track_indices;  // Suppress unused parameter warning
}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    // Your implementation here
    return std::vector<std::string>(); // Placeholder
}
