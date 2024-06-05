#pragma once

#include <JuceHeader.h>
#include "Graph.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  
    : public juce::Component,
    public juce::Button::Listener,
    public juce::Timer
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void updateValueFont();

    //==============================================================================
    void updateCounter(int newValue, bool force = false);

    //==============================================================================
    void buttonClicked(juce::Button* button) override;
    bool keyPressed(const juce::KeyPress& key) override;
    void timerCallback() override;
private:
    //==============================================================================
    // Your private member variables go here...
    static constexpr float margin = 50.f;
    float yLineValue;

    int contador;
    juce::ImageButton plusButton;

    juce::Image fondo;

    Graph graph;


    juce::Label titleText;
    juce::Label rondaText;
    juce::Label rondaValue;
    juce::Label literText;
    juce::Label literValue;

    juce::KeyPress sKey;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
