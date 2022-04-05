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
    ),
    treeState(*this, nullptr, "PARAMETER", { std::make_unique<AudioParameterFloat>(GAIN_ID, GAIN_NAME, NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f),
                        std::make_unique<AudioParameterFloat>(MASTER_ID, MASTER_NAME, NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5),
                        std::make_unique<AudioParameterFloat>(PANA_ID, PANA_NAME, NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.0f),
                        std::make_unique<AudioParameterFloat>(PANB_ID, PANB_NAME, NormalisableRange<float>(0.0f, 1.0f, 0.01f), 1.0f),
                        std::make_unique<AudioParameterFloat>(BALANCE_ID, BALANCE_NAME, NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f) })
#endif
{


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
	
    // Override stereo checkbox to mono if only 1 channel available
    if (numInputChannels < 2) {
        isStereo_override = false;
	numChannels = 1;
    } else {
        isStereo_override = isStereo;
	numChannels = 2;
    }

    auto block = dsp::AudioBlock<float>(buffer).getSingleChannelBlock(0);
    auto context = juce::dsp::ProcessContextReplacing<float>(block);
	
    // Copy channel 0 to channel 1 for dual mono input 
    if (isDualMono == true) {
        for (int ch = 1; ch < buffer.getNumChannels(); ++ch)
            buffer.copyFrom(ch, 0, buffer, 0, 0, buffer.getNumSamples());
    }

    // Amp =============================================================================
    if (amp_state == 1) {

        // Apply ramped changes for gain smoothing
        if (driveValue == previousDriveValue)
        {
            buffer.applyGain(driveValue * 2.0);
        }
        else {
            buffer.applyGainRamp(0, (int) buffer.getNumSamples(), previousDriveValue * 2.0, driveValue * 2.0);
            previousDriveValue = driveValue;
        }

        if (isStereo_override == false) {
            // Process IR-A only
            if (ira_state == true && irb_state == false && num_irs > 0) {
        
                cabSimIRa.process(context); // Process IR on channel 0

                // IR generally makes output quieter, add volume here to make ir on/off volume more even
                buffer.applyGain(2.0);

            // Process both IR-A and IR-B
            } else if (irb_state == true && ira_state == true && num_irs > 0) {
		    
		// Create temporary buffer for processing    
                auto buffer_temp = AudioBuffer<float>(1, numSamples);
                auto block_temp0 = dsp::AudioBlock<float>(buffer_temp).getSingleChannelBlock(0);
		auto context_temp = juce::dsp::ProcessContextReplacing<float>(block_temp0);

                // Process IR-A and IR-B on Channel 0, Channel 1 respectively
                cabSimIRa.process(context); // Process IR on channel 0
                cabSimIRb.process(context_temp); // Process IR on channel 0 data (temporary block)

                // Apply Balance Parameter //
                // This applies a balance to the mono output from both IR's.
                // For example, if Balance = 0.3 (from a range 0.0 to 1.0),
                //   then IR-A will share 30% of the mono output and 
                //   IR-B will share 70% of the mono output. 
            
                // Apply the appropriate Balance to each channel for mono output
                block.multiplyBy(1.0 - balanceValue);
                block_temp0.multiplyBy(balanceValue);

                // Add channel 1 to channel 0 (channel 0 will be copied to channel 1 later on)
                block.add(block_temp0);

            // Process IR-B only
            } else if (ira_state == false && irb_state == true && num_irs > 0) {
                cabSimIRb.process(context);
                buffer.applyGain(2.0);
            }

            // Copy channel 0 to channel 1 for mono output
            for (int ch = 1; ch < buffer.getNumChannels(); ++ch)
                buffer.copyFrom(ch, 0, buffer, 0, 0, buffer.getNumSamples());

        } else { // PROCESS STEREO / PANNING Controls //////////////////////////////////////////////////////////////

	    auto block2 = dsp::AudioBlock<float>(buffer).getSingleChannelBlock(1);
            auto context2 = juce::dsp::ProcessContextReplacing<float>(block2);
            
            // Process IR-A only
            if (ira_state == true && irb_state == false && num_irs > 0) {
            
                cabSimIRa.process(context); // Process IR on channel 0

                for (int ch = 1; ch < buffer.getNumChannels(); ++ch)
                    buffer.copyFrom(ch, 0, buffer, 0, 0, buffer.getNumSamples());

                block.multiplyBy(1.0 - panAValue);
                block2.multiplyBy(panAValue);

                // IR generally makes output quieter, add volume here to make ir on/off volume more even
                buffer.applyGain(2.0);

            }  else if (irb_state == true && ira_state == true && num_irs > 0) {
		 
		// Create temporary buffers for processing    
                auto buffer_temp = AudioBuffer<float>(2, numSamples);
                auto block_temp0 = dsp::AudioBlock<float>(buffer_temp).getSingleChannelBlock(0);
                auto block_temp1 = dsp::AudioBlock<float>(buffer_temp).getSingleChannelBlock(1);

                // Process IR-A and IR-B on Channel 0, Channel 1 respectively
                cabSimIRa.process(context); // Process IR on channel 0
                cabSimIRb.process(context2); // Process IR on channel 1

                // Copy original audio for channel 1 into temporary buffer
                block_temp0.copyFrom(block);
                block_temp1.copyFrom(block2);

                // Apply panning to channel 1
                block.multiplyBy(1.0 - panAValue);

                // Apply panning to channel 2
                block2.multiplyBy(panBValue);

                // Add channel 2 pan to channel 1
                block.add(block_temp1.multiplyBy(1.0 - panBValue));

                // Add channel 1 pan to channel 2
                block2.add(block_temp0.multiplyBy(panAValue));

            // Process IR-B only
            } else if (ira_state == false && irb_state == true && num_irs > 0) {
                cabSimIRb.process(context); // Process IR on channel 0

                for (int ch = 1; ch < buffer.getNumChannels(); ++ch)
                    buffer.copyFrom(ch, 0, buffer, 0, 0, buffer.getNumSamples());
                block.multiplyBy(1.0 - panBValue);
                block2.multiplyBy(panBValue);

                buffer.applyGain(2.0);
            }
        }

        // Master Volume 
        // Apply ramped changes for gain smoothing
        if (masterValue == previousMasterValue)
        {
            buffer.applyGain(masterValue * 2.0);
        }
        else {
            buffer.applyGainRamp(0, (int) buffer.getNumSamples(), previousMasterValue * 2.0, masterValue * 2.0);
            previousMasterValue = masterValue;
        }
    }
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

    auto state = treeState.copyState();
    std::unique_ptr<XmlElement> xml (state.createXml());
    xml->setAttribute ("ira_state", ira_state);
    xml->setAttribute ("irb_state", irb_state);
    xml->setAttribute ("isStereo", isStereo);
    copyXmlToBinary (*xml, destData);
}

void DualIRAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName (treeState.state.getType()))
        {
            treeState.replaceState (juce::ValueTree::fromXml (*xmlState));
	    ira_state = xmlState->getBoolAttribute ("ira_state");
            irb_state = xmlState->getBoolAttribute ("irb_state");
	    isStereo = xmlState->getBoolAttribute ("isStereo");
        }
    }
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

void DualIRAudioProcessor::setPanA(float paramPanA)
{
    panAValue = paramPanA;
}

void DualIRAudioProcessor::setPanB(float paramPanB)
{
    panBValue = paramPanB;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DualIRAudioProcessor();
}
