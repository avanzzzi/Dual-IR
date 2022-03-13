/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "CabSim.h"

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#define GAIN_ID "gain"
#define GAIN_NAME "Gain"
#define MASTER_ID "master"
#define MASTER_NAME "Master"
#define PANA_ID "pana"
#define PANA_NAME "Pan-A"
#define PANB_ID "panb"
#define PANB_NAME "Pan-B"
#define BALANCE_ID "balance"
#define BALANCE_NAME "Balance"

//==============================================================================
/**
*/
class DualIRAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    DualIRAudioProcessor();
    ~DualIRAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void loadIRa(File irFile);
    void loadIRb(File irFile);
    //void setupDataDirectories();

    //void addDirectoryIR(const File& file);
    //void resetDirectoryIR(const File& file);
    std::vector<File> irFiles;
    //File currentDirectory = File::getCurrentWorkingDirectory().getFullPathName();
    //File userAppDataDirectory = File::getSpecialLocation(File::userDocumentsDirectory).getChildFile(JucePlugin_Manufacturer).getChildFile(JucePlugin_Name);
    //File userAppDataDirectory_irs = userAppDataDirectory.getFullPathName() + "/irs";
    File userAppDataDirectory_irs = File::getSpecialLocation(File::userDesktopDirectory);

    // Pedal/amp states
    int amp_state = 1; // 0 = off, 1 = on

    juce::String loaded_ir_name;
    float num_irs = 0.0;
    int ira_loaded = 0;
    int irb_loaded = 0;

    File loaded_ira;
    File loaded_irb;
    bool ira_state = true;
    bool irb_state = true;

    int current_ira_index = 0;
    int current_irb_index = 0;

    void setDrive(float paramDrive);
    void setMaster(float db_ampMaster);
    void setBalance(float paramBalance);
    void setPanA(float paramPanA);
    void setPanB(float paramPanB);

    float driveValue = 0.5;
    float masterValue = 0.5;
    float balanceValue = 0.5;

    float previousDriveValue = 0.5;
    float previousMasterValue = 0.5;

    // Stereo and Pan params
    bool isStereo = false;
    float panAValue= 0.0;
    float panBValue = 1.0;

    AudioProcessorValueTreeState treeState;

private:
    var dummyVar;

    AudioParameterFloat* gainParam;
    AudioParameterFloat* masterParam;
    AudioParameterFloat* panaParam;
    AudioParameterFloat* panbParam;
    AudioParameterFloat* balanceParam;

    // IR processing
    CabSim cabSimIRa;
    CabSim cabSimIRb;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DualIRAudioProcessor)
};
