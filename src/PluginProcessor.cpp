/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <iostream>
#include <fstream>

//==============================================================================
DualIRAudioProcessor::DualIRAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", AudioChannelSet::stereo(), true)
#endif
    )

#endif
{
    setupDataDirectories();

    resetDirectoryIR(userAppDataDirectory_irs);
    // Sort iraFiles alphabetically
    std::sort(irFiles.begin(), irFiles.end());
    if (irFiles.size() > 0) {
        loadIRa(irFiles[current_ira_index]);
        loadIRb(irFiles[current_irb_index]);
    }



    // initialize parameters:
    addParameter(gainParam = new AudioParameterFloat(GAIN_ID, GAIN_NAME, NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));
    addParameter(masterParam = new AudioParameterFloat(MASTER_ID, MASTER_NAME, NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));
    //addParameter(bassParam = new AudioParameterFloat(PANA_ID, PANA_NAME, NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));
    //addParameter(midParam = new AudioParameterFloat(PANB_ID, PANB_NAME, NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));
    addParameter(balanceParam = new AudioParameterFloat(BALANCE_ID, BALANCE_NAME, NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));
}


DualIRAudioProcessor::~DualIRAudioProcessor()
{
}

//==============================================================================
const String DualIRAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DualIRAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DualIRAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DualIRAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DualIRAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DualIRAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DualIRAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DualIRAudioProcessor::setCurrentProgram (int index)
{
}

const String DualIRAudioProcessor::getProgramName (int index)
{
    return {};
}

void DualIRAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void DualIRAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    // Set up IR
    dsp::ProcessSpec spec{ sampleRate, static_cast<uint32> (samplesPerBlock), 2 };
    cabSimIRa.prepare(spec);
    cabSimIRb.prepare(spec);
}

void DualIRAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DualIRAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif


void DualIRAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;

    // Setup Audio Data
    const int numSamples = buffer.getNumSamples();
    const int numInputChannels = getTotalNumInputChannels();
    const int sampleRate = getSampleRate();

    auto block = dsp::AudioBlock<float>(buffer).getSingleChannelBlock(0);
    auto context = juce::dsp::ProcessContextReplacing<float>(block);

    auto block2 = dsp::AudioBlock<float>(buffer).getSingleChannelBlock(1);
    auto context2 = juce::dsp::ProcessContextReplacing<float>(block2);

    // Amp =============================================================================
    if (amp_state == 1) {

        buffer.applyGain(driveValue * 2.0);

        // Process IR-A
        if (ira_state == true && irb_state == false && num_irs > 0) {
        
            cabSimIRa.process(context); // Process IR on channel 0

            // IR generally makes output quieter, add volume here to make ir on/off volume more even
            buffer.applyGain(2.0);
        }

        // Process IR-B
        if (irb_state == true && ira_state == true && num_irs > 0) {

            // Process IR-A and IR-B on Channel 0, Channel 1 respectively
            cabSimIRa.process(context); // Process IR on channel 0
            cabSimIRb.process(context2); // Process IR on channel 1


            // Apply Balance Parameter //
            // This applies a balance to the mono output from both IR's.
            // For example, if Balance = 0.3 (from a range 0.0 to 1.0),
            //   then IR-A will share 30% of the mono output and 
            //   IR-B will share 70% of the mono output. 

            // Apply the appropriate Balance to each channel
            block.multiplyBy(1.0 - balanceValue); // TODO Verify that multiplyBy function acts like a Gain
            block2.multiplyBy(balanceValue);

            // Add channel 1 to channel 0 (channel 0 will be copied to channel 1 later on)
            block.add(block2);


        } else if (ira_state == false && irb_state == true && num_irs > 0) {
            cabSimIRb.process(context);
            buffer.applyGain(2.0);
        }

        //    Master Volume 
	buffer.applyGain(masterValue * 2.0);
    }


    // Copy Channel 0 to Channel 1 for Mono Output
    for (int ch = 1; ch < buffer.getNumChannels(); ++ch)
        buffer.copyFrom(ch, 0, buffer, 0, 0, buffer.getNumSamples());
}

//==============================================================================
bool DualIRAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DualIRAudioProcessor::createEditor()
{
    return new DualIRAudioProcessorEditor (*this);
}

void DualIRAudioProcessor::getStateInformation(MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

}

void DualIRAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

}

void DualIRAudioProcessor::loadIRa(File irFile)
{
    this->suspendProcessing(true);

    try {
        cabSimIRa.load(irFile);
        ira_loaded = 1;
    }
    catch (const std::exception& e) {
        DBG("Unable to load IR file: " + irFile.getFullPathName());
        std::cout << e.what();
    }
    this->suspendProcessing(false);
}


void DualIRAudioProcessor::loadIRb(File irFile)
{
    this->suspendProcessing(true);

    try {
        cabSimIRb.load(irFile);
        irb_loaded = 1;
    }
    catch (const std::exception& e) {
        DBG("Unable to load IR file: " + irFile.getFullPathName());
        std::cout << e.what();
    }
    this->suspendProcessing(false);
}


void DualIRAudioProcessor::resetDirectoryIR(const File& file)
{
    irFiles.clear();
    if (file.isDirectory())
    {
        juce::Array<juce::File> results;
        file.findChildFiles(results, juce::File::findFiles, false, "*.wav");
        for (int i = results.size(); --i >= 0;)
            irFiles.push_back(File(results.getReference(i).getFullPathName()));
    }
}


void DualIRAudioProcessor::addDirectoryIR(const File& file)
{
    if (file.isDirectory())
    {
        juce::Array<juce::File> results;
        file.findChildFiles(results, juce::File::findFiles, false, "*.wav");
        for (int i = results.size(); --i >= 0;)
        {
            irFiles.push_back(File(results.getReference(i).getFullPathName()));
            num_irs = num_irs + 1.0;
        }
    }
}

void DualIRAudioProcessor::setupDataDirectories()
{
    // User app data directory
    File userAppDataTempFile = userAppDataDirectory.getChildFile("tmp.pdl");

    File userAppDataTempFile_irs = userAppDataDirectory_irs.getChildFile("tmp.pdl");

    // Create (and delete) temp file if necessary, so that user doesn't have
    // to manually create directories
    if (!userAppDataDirectory.exists()) {
        userAppDataTempFile.create();
    }
    if (userAppDataTempFile.existsAsFile()) {
        userAppDataTempFile.deleteFile();
    }


    if (!userAppDataDirectory_irs.exists()) {
        userAppDataTempFile_irs.create();
    }
    if (userAppDataTempFile_irs.existsAsFile()) {
        userAppDataTempFile_irs.deleteFile();
    }

    // Add the tones directory and update IR list
    addDirectoryIR(userAppDataDirectory_irs);
}

void DualIRAudioProcessor::setDrive(float paramDrive)
{
    driveValue = paramDrive;
}


void DualIRAudioProcessor::setMaster(float db_ampMaster)
{
    masterValue = db_ampMaster;
}

void DualIRAudioProcessor::setBalance(float paramBalance)
{
    balanceValue = paramBalance;
}


/*
float DualIRAudioProcessor::convertLogScale(float in_value, float x_min, float x_max, float y_min, float y_max)
{
    float b = log(y_max / y_min) / (x_max - x_min);
    float a = y_max / exp(b * x_max);
    float converted_value = a * exp(b * in_value);
    return converted_value;
}


float DualIRAudioProcessor::decibelToLinear(float dbValue)
{
    return powf(10.0, dbValue/20.0);
}
*/

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DualIRAudioProcessor();
}
