/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include <stdlib.h>

//==============================================================================
/**
*/
class DualIRAudioProcessorEditor  : public AudioProcessorEditor,
                                      private Button::Listener,
                                      private Slider::Listener
                                
{
public:
    DualIRAudioProcessorEditor (DualIRAudioProcessor&);
    ~DualIRAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    std::unique_ptr<FileChooser> myChooser;
    
    void resetMode();
    void modeSelectChanged();
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DualIRAudioProcessor& processor;

    Image background = ImageCache::getFromMemory(BinaryData::npi_background_jpg, BinaryData::npi_background_jpgSize);

    // Amp Widgets
    Slider ampGainKnob;
    Slider ampMasterKnob;
    ComboBox iraSelect;
    ComboBox irbSelect;
    ComboBox modeSelect;
    Slider ampPanAKnob;
    Slider ampPanBKnob;
    Slider ampBalanceKnob;

    Label GainLabel;
    Label LevelLabel;
    Label PanALabel;
    Label PanBLabel;
    Label BalanceLabel;
    Label BalanceLabelA;
    Label BalanceLabelB;

    Label iraDropDownLabel;
    Label irbDropDownLabel;
    Label modeLabel;
    Label versionLabel;

    File test_file;

    TextButton loadIR;
    ToggleButton iraButton;
    ToggleButton irbButton;

    juce::LookAndFeel_V4 blueLookAndFeel;
    juce::LookAndFeel_V4 greyLookAndFeel;
    
    juce::String fname;
    virtual void buttonClicked(Button* button) override;
    void updateToggleState(juce::Button* button, juce::String name);
    void iraSelectChanged();
    void irbSelectChanged();

    void loadIRClicked();
    virtual void sliderValueChanged(Slider* slider) override;

public:
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> gainSliderAttach;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> masterSliderAttach;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> panaSliderAttach;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> panbSliderAttach;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> balanceSliderAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DualIRAudioProcessorEditor)
};
