#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
    : decks(), active_deck(1), auto_sync(false), bpm_tolerance(0)
{
    decks[0] = nullptr;
    decks[1] = nullptr;
    std::cout << "[MixingEngineService] Initialized with 2 empty decks." << std::endl;
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService() {
    std::cout << "[MixingEngineService] Cleaning up decks..." << std::endl;
    for (int i = 0; i < 2; i++) {
        if (decks[i]) {
            delete decks[i];
            decks[i] = nullptr;
        }
    }
}


/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    std::cout << "\n=== Loading Track to Deck ===" << std::endl;

    PointerWrapper<AudioTrack> temp_ptr = track.clone();
    if (!temp_ptr) {
        std::cerr << "[ERROR] Track: \"" << track.get_title() << "\" failed to clone" << std::endl;
        return -1;
    }
    AudioTrack* cloned_track = temp_ptr.release();
    PointerWrapper<AudioTrack> wrapped_track(cloned_track);

    size_t target_deck = 1 - active_deck;
    if (decks[0] == nullptr && decks[1] == nullptr) {
        target_deck = 0; 
        active_deck = 0; 
    }
    std::cout << "[Deck Switch] Target deck: " << target_deck << std::endl;

    if (decks[target_deck] != nullptr) {
        delete decks[target_deck];
        decks[target_deck] = nullptr;
    }
    wrapped_track->load();
    wrapped_track->analyze_beatgrid();
    if (decks[active_deck] != nullptr) {
        if (!can_mix_tracks(wrapped_track) && auto_sync)
            sync_bpm(wrapped_track);
    }
    else {
        std::cout << "[Sync BPM] Cannot sync - one of the decks is empty." << std::endl;
    }

    AudioTrack* released_track = wrapped_track.release();
    decks[target_deck] = released_track;
    std::cout << "[Load Complete] '" << released_track->get_title() << "' is now loaded on deck " << target_deck << std::endl;
    active_deck = target_deck;
    std::cout << "[Active Deck] Switched to deck " << target_deck << std::endl;
    displayDeckStatus();

    return target_deck;
}

/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const {
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; ++i) {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 * TODO: Implement can_mix_tracks method
 * 
 * Check if two tracks can be mixed based on BPM difference.
 * 
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
    if(decks[active_deck] == nullptr || !track)
        return false;
    int bpm1 = track->get_bpm();
    int bpm2 = decks[active_deck]->get_bpm();
    if(std::abs(bpm1-bpm2) <= bpm_tolerance)
        return true;
    return false;
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    if(decks[active_deck] == nullptr || !track)
        return;
    int bpm1 = track->get_bpm();
    double avg = (bpm1 + decks[active_deck]->get_bpm()) /2;
    int newbpm = (int)(avg);
    track->set_bpm(newbpm);
    std::cout << "[Sync BPM] Syncing BPM from " << bpm1 << " to " << newbpm << std::endl;
}
