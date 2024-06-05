/*
  ==============================================================================

    Graph.h
    Created: 5 Jun 2024 11:45:15am
    Author:  marcs

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class Graph  : public juce::Component
{
public:
    Graph()
        :maxValue(0)
    {
    }

    ~Graph() override
    {
    }

    void clear()
    {
        maxValue = 0;
        dataPoints.clear();
        repaint();
    }

    void addValue(int newValue)
    {
        if (newValue > maxValue)
            maxValue = newValue;

        dataPoints.add(DataPoint(newValue));
        repaint();

    }

    void paint (juce::Graphics& g) override
    {
        g.setColour(juce::Colours::red.withAlpha(0.8f));
        juce::Path p = getGraph();
        g.fillPath(p);
        g.setColour(juce::Colours::black);
        g.strokePath(p, juce::PathStrokeType(strokeWidth));
    }

    void resized() override
    {
    }

private:
    static constexpr int strokeWidth = 4;

    juce::Path getGraph() const
    {
         juce::Path path;
        const float minY = (float)getHeight();

        if (!dataPoints.isEmpty())
        {
    
            float xZero = (float)dataPoints.getFirst().time;
            float xRange = (float)dataPoints.getLast().time - xZero;
    
            path.startNewSubPath(0, minY);
            float x = 0, y = 0;
            for(auto& dp : dataPoints)
            {
                x = &dp == dataPoints.begin() ? 0 : (dp.time - xZero) / xRange * getWidth();
                y = maxValue == 0 ? minY : minY - (minY * dp.value / (float) maxValue) + strokeWidth * 0.5f; // Half stroke
                path.lineTo(x, y);
            }
            path.lineTo((float)getWidth(), y);
            path.lineTo((float)getWidth(), minY);
            path.closeSubPath();
        }
       return path;
    }

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


    int maxValue;
    juce::Array<DataPoint> dataPoints;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Graph)
};
