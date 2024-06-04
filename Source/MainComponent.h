#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  
    : public juce::Component,
    public juce::Button::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    //==============================================================================
    void updateCounter(int newValue, bool force = false);

    //==============================================================================
    void buttonClicked(juce::Button* button) override;
    bool keyPressed(const juce::KeyPress& key) override;
private:
    //==============================================================================
    // Your private member variables go here...
    int contador;
    juce::ImageButton plusButton;

    juce::Image fondo;

    juce::Label titleText;
    juce::Label rondaText;
    juce::Label rondaValue;
    juce::Label literText;
    juce::Label literValue;

    juce::KeyPress sKey;

    struct DataPoint 
    {
        DataPoint(int counter)
        {
            time = juce::Time::getApproximateMillisecondCounter();
            value = counter;
        }

        DataPoint()
            :time(0), value(0)
        {

        }
        int time;
        int value;
    };

    bool needsPaint = false;

    juce::Array<DataPoint> dataPoints;
    juce::Path getGraph();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
