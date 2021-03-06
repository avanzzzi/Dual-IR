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


    blueLookAndFeel.setColour(juce::Slider::thumbColourId, juce::Colours::aqua);
    greyLookAndFeel.setColour(juce::Slider::thumbColourId, juce::Colours::grey);
    
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

    addAndMakeVisible(modeSelect);
    modeSelect.setColour(juce::Label::textColourId, juce::Colours::black);
    modeSelect.addItem("Mono", 1);
    modeSelect.addItem("Stereo", 2);      
    modeSelect.onChange = [this] {modeSelectChanged(); };
    modeSelect.setSelectedItemIndex(0, juce::NotificationType::dontSendNotification);
    modeSelect.setScrollWheelEnabled(true);

    addAndMakeVisible(loadIR);
    loadIR.setButtonText("Import IR");
    loadIR.setColour(juce::Label::textColourId, juce::Colours::black);
    loadIR.addListener(this);

    // Toggle IR-A
    addAndMakeVisible(iraButton); // Toggle is for testing purposes
    iraButton.setToggleState(true, juce::NotificationType::dontSendNotification);
    iraButton.onClick = [this] { updateToggleState(&iraButton, "IRA");   };

    // Toggle IR-B
    addAndMakeVisible(irbButton); // Toggle is for testing purposes
    irbButton.setToggleState(true, juce::NotificationType::dontSendNotification);
    irbButton.onClick = [this] { updateToggleState(&irbButton, "IRB");   };

    gainSliderAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, GAIN_ID, ampGainKnob);
    addAndMakeVisible(ampGainKnob);
    ampGainKnob.setLookAndFeel(&blueLookAndFeel);
    ampGainKnob.addListener(this);
    ampGainKnob.setRange(0.0, 1.0);
    ampGainKnob.setValue(processor.driveValue);
    ampGainKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampGainKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    ampGainKnob.setNumDecimalPlacesToDisplay(2);
    ampGainKnob.setDoubleClickReturnValue(true, 0.5);

    masterSliderAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, MASTER_ID, ampMasterKnob);
    addAndMakeVisible(ampMasterKnob);
    ampMasterKnob.setLookAndFeel(&blueLookAndFeel);
    ampMasterKnob.addListener(this);
    ampMasterKnob.setRange(0.0, 1.0);
    ampMasterKnob.setValue(processor.masterValue);
    ampMasterKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampMasterKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    ampMasterKnob.setNumDecimalPlacesToDisplay(2);
    ampMasterKnob.setDoubleClickReturnValue(true, 0.5);

    panaSliderAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, PANA_ID, ampPanAKnob);
    addAndMakeVisible(ampPanAKnob);
    ampPanAKnob.setLookAndFeel(&greyLookAndFeel);
    ampPanAKnob.addListener(this);
    ampPanAKnob.setRange(0.0, 1.0);
    ampPanAKnob.setValue(processor.panAValue);
    ampPanAKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampPanAKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    ampPanAKnob.setNumDecimalPlacesToDisplay(2);
    ampPanAKnob.setDoubleClickReturnValue(true, 0.5);
    ampPanBKnob.setEnabled(false);

    panbSliderAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, PANB_ID, ampPanBKnob);
    addAndMakeVisible(ampPanBKnob);
    ampPanBKnob.setLookAndFeel(&greyLookAndFeel);
    ampPanBKnob.addListener(this);
    ampPanBKnob.setRange(0.0, 1.0);
    ampPanBKnob.setValue(processor.panBValue);
    ampPanBKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampPanBKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    ampPanBKnob.setNumDecimalPlacesToDisplay(2);
    ampPanBKnob.setDoubleClickReturnValue(true, 0.5);
    ampPanBKnob.setEnabled(false);

    balanceSliderAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, BALANCE_ID, ampBalanceKnob);
    addAndMakeVisible(ampBalanceKnob);
    ampBalanceKnob.setLookAndFeel(&blueLookAndFeel);
    ampBalanceKnob.addListener(this);
    ampBalanceKnob.setRange(0.0, 1.0);
    ampBalanceKnob.setValue(processor.balanceValue);
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

    addAndMakeVisible(PanALabel);
    PanALabel.setText("Pan-A", juce::NotificationType::dontSendNotification);
    PanALabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(BalanceLabelA);
    BalanceLabelA.setText("A", juce::NotificationType::dontSendNotification);
    BalanceLabelA.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(BalanceLabelB);
    BalanceLabelB.setText("B", juce::NotificationType::dontSendNotification);
    BalanceLabelB.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(PanBLabel);
    PanBLabel.setText("Pan-B", juce::NotificationType::dontSendNotification);
    PanBLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(iraDropDownLabel);
    iraDropDownLabel.setText("IR-A", juce::NotificationType::dontSendNotification);
    iraDropDownLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(irbDropDownLabel);
    irbDropDownLabel.setText("IR-B", juce::NotificationType::dontSendNotification);
    irbDropDownLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(modeLabel);
    modeLabel.setText("Mode", juce::NotificationType::dontSendNotification);
    modeLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(versionLabel);
    versionLabel.setText("v1.0.0", juce::NotificationType::dontSendNotification);
    versionLabel.setJustificationType(juce::Justification::centred);

    auto font = GainLabel.getFont();
    float height = font.getHeight();
    font.setHeight(height); // 0.75);
    GainLabel.setFont(font);
    LevelLabel.setFont(font);

    BalanceLabel.setFont(font);
    PanALabel.setFont(font);
    PanBLabel.setFont(font);
    BalanceLabelA.setFont(font);
    BalanceLabelB.setFont(font);
    modeLabel.setFont(font);

    iraDropDownLabel.setFont(font);
    irbDropDownLabel.setFont(font);
    versionLabel.setFont(font);

    // Size of plugin GUI
    setSize(420, 230);

    resetMode();
    modeSelectChanged();
}

DualIRAudioProcessorEditor::~DualIRAudioProcessorEditor()
{
    ampGainKnob.setLookAndFeel(nullptr);
    ampBalanceKnob.setLookAndFeel(nullptr);
    ampMasterKnob.setLookAndFeel(nullptr);
    ampPanAKnob.setLookAndFeel(nullptr);
    ampPanBKnob.setLookAndFeel(nullptr);
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
    iraButton.setBounds(325, 8, 30, 30);
    irbButton.setBounds(325, 40, 30, 30);
    modeSelect.setBounds(330, 71, 80, 30);

    // Amp Widgets
    ampGainKnob.setBounds(10, 120, 75, 95);
    ampMasterKnob.setBounds(95, 120, 75, 95);
    ampBalanceKnob.setBounds(180, 120, 75, 95);
    ampPanAKnob.setBounds(265, 120, 75, 95);
    ampPanBKnob.setBounds(350, 120, 75, 95);
   
    GainLabel.setBounds(6, 108, 80, 10);
    LevelLabel.setBounds(93, 108, 80, 10);
    BalanceLabel.setBounds(180, 108, 80, 10);
    PanALabel.setBounds(265, 108, 80, 10);
    PanBLabel.setBounds(350, 108, 80, 10);
    BalanceLabelA.setBounds(178, 170, 20, 20);
    BalanceLabelB.setBounds(238, 171, 20, 20);
 
    iraDropDownLabel.setBounds(280, 16, 40, 10);
    irbDropDownLabel.setBounds(280, 48, 40, 10);
    modeLabel.setBounds(275, 80, 60, 10);
    versionLabel.setBounds(340, 8, 80, 10);

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

void DualIRAudioProcessorEditor::modeSelectChanged()
{
    const int modeIndex = modeSelect.getSelectedItemIndex();
    if (modeIndex == 0) {
        processor.isStereo = false;
        ampPanAKnob.setLookAndFeel(&greyLookAndFeel);
        ampPanBKnob.setLookAndFeel(&greyLookAndFeel);
        ampBalanceKnob.setLookAndFeel(&blueLookAndFeel);
        ampPanAKnob.setEnabled(false);
        ampPanBKnob.setEnabled(false);
        ampBalanceKnob.setEnabled(true);
  
    } else if (modeIndex == 1) {
        processor.isStereo = true;
        ampBalanceKnob.setLookAndFeel(&greyLookAndFeel);
        ampPanAKnob.setLookAndFeel(&blueLookAndFeel);
        ampPanBKnob.setLookAndFeel(&blueLookAndFeel);
        ampPanAKnob.setEnabled(true);
        ampPanBKnob.setEnabled(true);
        ampBalanceKnob.setEnabled(false);
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

    else if (name == "IRB")
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
    //FileChooser chooser("Select one or more .wav IR files to import",
    //    {},
    //    "*.wav");
    myChooser = std::make_unique<FileChooser> ("Select one or more .wav IR files to import",
                                               File::getSpecialLocation (File::userDesktopDirectory),
                                               "*.wav");
 
    auto folderChooserFlags = FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles | FileBrowserComponent::canSelectMultipleItems;
 
    myChooser->launchAsync (folderChooserFlags, [this] (const FileChooser& chooser)                
    {
        //int import_fail = 1;
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
    });
    
}

void DualIRAudioProcessorEditor::resetMode()
{
    if (processor.isStereo == false) {
        modeSelect.setSelectedItemIndex(0, juce::NotificationType::dontSendNotification);
    } else {
        modeSelect.setSelectedItemIndex(1, juce::NotificationType::dontSendNotification);
    }

    if (processor.ira_state == false) {
        iraButton.setToggleState(false, juce::NotificationType::dontSendNotification);
    } else {
        iraButton.setToggleState(true, juce::NotificationType::dontSendNotification);
    }

    if (processor.irb_state == false) {
        irbButton.setToggleState(false, juce::NotificationType::dontSendNotification);
    } else {
        irbButton.setToggleState(true, juce::NotificationType::dontSendNotification);
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
    else if (slider == &ampPanAKnob)
        processor.setPanA(slider->getValue());
    else if (slider == &ampPanBKnob)
        processor.setPanB(slider->getValue());
}
