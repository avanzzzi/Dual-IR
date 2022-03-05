/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <stdio.h>
#include <fstream>
#include <iostream>


//==============================================================================
DualIRAudioProcessorEditor::DualIRAudioProcessorEditor (DualIRAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to

    
    addAndMakeVisible(iraSelect);
    iraSelect.setColour(juce::Label::textColourId, juce::Colours::black);
    int c = 1;
    for (const auto& irFile : processor.irFiles) {
        iraSelect.addItem(irFile.getFileNameWithoutExtension(), c);
        c += 1;
    }
    iraSelect.onChange = [this] {iraSelectChanged(); };
    iraSelect.setSelectedItemIndex(processor.current_ira_index, juce::NotificationType::dontSendNotification);
    iraSelect.setScrollWheelEnabled(true);


    addAndMakeVisible(irbSelect);
    irbSelect.setColour(juce::Label::textColourId, juce::Colours::black);
    int i = 1;
    for (const auto& irFile : processor.irFiles) {
        irbSelect.addItem(irFile.getFileNameWithoutExtension(), i);
        i += 1;
    }
    irbSelect.onChange = [this] {irbSelectChanged(); };
    irbSelect.setSelectedItemIndex(processor.current_irb_index, juce::NotificationType::dontSendNotification);
    irbSelect.setScrollWheelEnabled(true);

    addAndMakeVisible(loadIR);
    loadIR.setButtonText("Import IR");
    loadIR.setColour(juce::Label::textColourId, juce::Colours::black);
    loadIR.addListener(this);

    // Toggle IR
    addAndMakeVisible(iraButton); // Toggle is for testing purposes
    iraButton.setToggleState(true, juce::NotificationType::dontSendNotification);
    iraButton.onClick = [this] { updateToggleState(&iraButton, "IRA");   };

    // Toggle LSTM
    addAndMakeVisible(irbButton); // Toggle is for testing purposes
    irbButton.setToggleState(true, juce::NotificationType::dontSendNotification);
    irbButton.onClick = [this] { updateToggleState(&irbButton, "IRB");   };
  

    addAndMakeVisible(ampGainKnob);
    //ampGainKnob.setLookAndFeel(&blueLookAndFeel);
    ampGainKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    ampGainKnob.setNumDecimalPlacesToDisplay(1);
    ampGainKnob.addListener(this);
    ampGainKnob.setRange(0.0, 1.0);
    ampGainKnob.setValue(0.5);
    ampGainKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampGainKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    ampGainKnob.setNumDecimalPlacesToDisplay(2);
    ampGainKnob.setDoubleClickReturnValue(true, 0.5);


    addAndMakeVisible(ampMasterKnob);
    //ampMasterKnob.setLookAndFeel(&blueLookAndFeel);
    ampMasterKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    ampMasterKnob.setNumDecimalPlacesToDisplay(1);
    ampMasterKnob.addListener(this);
    ampMasterKnob.setRange(0.0, 1.0);
    ampMasterKnob.setValue(0.5);
    ampMasterKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampMasterKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    ampMasterKnob.setNumDecimalPlacesToDisplay(2);
    //ampMasterKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 50, 20 );
    //ampMasterKnob.setNumDecimalPlacesToDisplay(1);
    ampMasterKnob.setDoubleClickReturnValue(true, 0.5);

    addAndMakeVisible(ampBalanceKnob);
    //ampGainKnob.setLookAndFeel(&blueLookAndFeel);
    ampBalanceKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    ampBalanceKnob.setNumDecimalPlacesToDisplay(1);
    ampBalanceKnob.addListener(this);
    ampBalanceKnob.setRange(0.0, 1.0);
    ampBalanceKnob.setValue(0.5);
    ampBalanceKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampBalanceKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    ampBalanceKnob.setNumDecimalPlacesToDisplay(2);
    ampBalanceKnob.setDoubleClickReturnValue(true, 0.5);


    addAndMakeVisible(GainLabel);
    GainLabel.setText("Gain", juce::NotificationType::dontSendNotification);
    GainLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(LevelLabel);
    LevelLabel.setText("Level", juce::NotificationType::dontSendNotification);
    LevelLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(BalanceLabel);
    BalanceLabel.setText("Balance", juce::NotificationType::dontSendNotification);
    BalanceLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(BalanceLabelA);
    BalanceLabelA.setText("A", juce::NotificationType::dontSendNotification);
    BalanceLabelA.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(BalanceLabelB);
    BalanceLabelB.setText("B", juce::NotificationType::dontSendNotification);
    BalanceLabelB.setJustificationType(juce::Justification::centred);


    addAndMakeVisible(iraDropDownLabel);
    iraDropDownLabel.setText("IR-A", juce::NotificationType::dontSendNotification);
    iraDropDownLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(irbDropDownLabel);
    irbDropDownLabel.setText("IR-B", juce::NotificationType::dontSendNotification);
    irbDropDownLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(versionLabel);
    versionLabel.setText("v1.0.0", juce::NotificationType::dontSendNotification);
    versionLabel.setJustificationType(juce::Justification::centred);

    auto font = GainLabel.getFont();
    float height = font.getHeight();
    font.setHeight(height); // 0.75);
    GainLabel.setFont(font);
    LevelLabel.setFont(font);
    BalanceLabel.setFont(font);
    BalanceLabelA.setFont(font);
    BalanceLabelB.setFont(font);

    iraDropDownLabel.setFont(font);
    irbDropDownLabel.setFont(font);
    versionLabel.setFont(font);


    // Size of plugin GUI
    setSize(380, 230);
}

DualIRAudioProcessorEditor::~DualIRAudioProcessorEditor()
{
}

//==============================================================================
void DualIRAudioProcessorEditor::paint (Graphics& g)
{
    // Workaround for graphics on Windows builds (clipping code doesn't work correctly on Windows)
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    g.drawImageAt(background, 0, 0);  // Debug Line: Redraw entire background image
#else
// Redraw only the clipped part of the background image
    juce::Rectangle<int> ClipRect = g.getClipBounds();
    g.drawImage(background, ClipRect.getX(), ClipRect.getY(), ClipRect.getWidth(), ClipRect.getHeight(), ClipRect.getX(), ClipRect.getY(), ClipRect.getWidth(), ClipRect.getHeight());
#endif
   
}

void DualIRAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    iraSelect.setBounds(11, 10, 270, 25);
   
    irbSelect.setBounds(11, 42, 270, 25);
    loadIR.setBounds(11, 74, 100, 25);
    iraButton.setBounds(335, 10, 30, 30);
    irbButton.setBounds(335, 42, 30, 30);

    // Amp Widgets
    ampGainKnob.setBounds(10, 120, 75, 95);
    ampMasterKnob.setBounds(95, 120, 75, 95);
    ampBalanceKnob.setBounds(180, 120, 75, 95);
   
    GainLabel.setBounds(6, 108, 80, 10);
    LevelLabel.setBounds(93, 108, 80, 10);
    BalanceLabel.setBounds(180, 108, 80, 10);
    BalanceLabelA.setBounds(177, 171, 20, 20);
    BalanceLabelB.setBounds(238, 171, 20, 20);
 

    iraDropDownLabel.setBounds(280, 16, 40, 10);
    irbDropDownLabel.setBounds(280, 48, 40, 10);
    versionLabel.setBounds(280, 210, 80, 10);

}

void DualIRAudioProcessorEditor::iraSelectChanged()
{
    const int selectedFileIndex = iraSelect.getSelectedItemIndex();
    if (selectedFileIndex >= 0 && selectedFileIndex < processor.irFiles.size()) {
        File selectedFile = processor.userAppDataDirectory_irs.getFullPathName() + "/" + iraSelect.getText() + ".wav";
        //processor.loadConfig(processor.irFiles[selectedFileIndex]);
        processor.loadIRa(selectedFile);
        processor.current_ira_index = selectedFileIndex;
    }
   
}

void DualIRAudioProcessorEditor::irbSelectChanged()
{
    const int selectedFileIndex = irbSelect.getSelectedItemIndex();
    if (selectedFileIndex >= 0 && selectedFileIndex < processor.irFiles.size()) {
        File selectedFile = processor.userAppDataDirectory_irs.getFullPathName() + "/" + irbSelect.getText() + ".wav";
        //processor.loadIR(processor.irFiles[selectedFileIndex]);
        processor.loadIRb(selectedFile);
        processor.current_irb_index = selectedFileIndex;
    }
}

void DualIRAudioProcessorEditor::updateToggleState(juce::Button* button, juce::String name)
{
    if (name == "IRA")
        //processor.ira_state = button->getToggleState();
        if (processor.ira_state == true) {
            processor.ira_state = false;
        } else {
            processor.ira_state = true;
        }

    else
        //processor.irb_state = button->getToggleState();
        if (processor.irb_state == true) {
            processor.irb_state = false;
        }
        else {
            processor.irb_state = true;
        }
}


void DualIRAudioProcessorEditor::loadIRClicked()
{
    FileChooser chooser("Select one or more .wav IR files to import",
        {},
        "*.wav");
    
    if (chooser.browseForMultipleFilesToOpen())
    {
        int import_fail = 1;
        Array<File> files = chooser.getResults();
        for (auto file : files) {
            File fullpath = processor.userAppDataDirectory_irs.getFullPathName() + "/" + file.getFileName();
            bool b = fullpath.existsAsFile();
            if (b == false) {

                //processor.loadIR(file);
                //fname = file.getFileName();
                //processor.loaded_ir = file;
                //processor.loaded_ir_name = fname;
                //processor.custom_ir = 1;

                // Copy selected file to model directory and load into dropdown menu
                bool a = file.copyFileTo(fullpath);
                if (a == true) {
                    // Add to ir-a menu
                    iraSelect.addItem(file.getFileNameWithoutExtension(), processor.irFiles.size() + 1);
                    //iraSelect.setSelectedItemIndex(processor.irFiles.size(), juce::NotificationType::dontSendNotification);

                    // Add to ir-b menu
                    irbSelect.addItem(file.getFileNameWithoutExtension(), processor.irFiles.size() + 1);
                    //irbSelect.setSelectedItemIndex(processor.irFiles.size(), juce::NotificationType::dontSendNotification);

                    processor.irFiles.push_back(file);
                    processor.num_irs += 1;
                }
                // Sort jsonFiles alphabetically
                //std::sort(processor.irFiles.begin(), processor.irFiles.end());
            }
        }
    }
    
}


void DualIRAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    if (button == &loadIR) {
        loadIRClicked();
    }
}


void DualIRAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    if (slider == &ampGainKnob)
        processor.setDrive(slider->getValue());
    else if (slider == &ampMasterKnob)
        processor.setMaster(slider->getValue());
    else if (slider == &ampBalanceKnob)
        processor.setBalance(slider->getValue());
    /*
    if (slider == &modelKnob) {
        if (slider->getValue() >= 0 && slider->getValue() < processor.jsonFiles.size()) {
            modelSelect.setSelectedItemIndex(processor.getModelIndex(slider->getValue()), juce::NotificationType::dontSendNotification);
        }
    } else if (slider == &irKnob) {
        if (slider->getValue() >= 0 && slider->getValue() < processor.irFiles.size()) {
            irSelect.setSelectedItemIndex(processor.getIrIndex(slider->getValue()), juce::NotificationType::dontSendNotification);
        }
    }
    */
}


