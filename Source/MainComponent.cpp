#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
    : contador(0), sKey('s'), plusButton("plusButton")
{

    setWantsKeyboardFocus(true);
    setRepaintsOnMouseActivity(false);

    juce::Image chapam1 = juce::ImageCache::getFromMemory(BinaryData::chapa1_png, BinaryData::chapa1_pngSize);
    fondo = juce::ImageCache::getFromMemory(BinaryData::rondadora_png, BinaryData::rondadora_pngSize);

    // Ronda Text
    addAndMakeVisible(rondaText);
    rondaText.setText("Botellas 33cl", juce::NotificationType::dontSendNotification);
    rondaText.setFont(juce::Font(80.f));
    rondaText.setColour(juce::Label::ColourIds::textColourId, juce::Colours::black);
    rondaText.setJustificationType(juce::Justification::centredLeft);
    rondaText.setRepaintsOnMouseActivity(false);

    // Ronda Value
    addAndMakeVisible(rondaValue);
    rondaValue.setFont(juce::Font(400.f).boldened());
    rondaValue.setColour(juce::Label::ColourIds::textColourId, juce::Colours::red);
    rondaValue.setJustificationType(juce::Justification::centredLeft);
    rondaValue.setRepaintsOnMouseActivity(false);

    // Litros Text
    addAndMakeVisible(literText);
    literText.setText("Litros Totales", juce::NotificationType::dontSendNotification);
    literText.setFont(juce::Font(80.f));
    literText.setColour(juce::Label::ColourIds::textColourId, juce::Colours::black);
    literText.setJustificationType(juce::Justification::centredRight);
    literText.setRepaintsOnMouseActivity(false);

    // Litros Value
    addAndMakeVisible(literValue);
    literValue.setFont(juce::Font(400.f).boldened());
    literValue.setColour(juce::Label::ColourIds::textColourId, juce::Colours::red);
    literValue.setJustificationType(juce::Justification::centredRight);
    literValue.setRepaintsOnMouseActivity(false);

    // Title Text
    addAndMakeVisible(titleText);
    titleText.setFont(juce::Font(110.f).boldened());
    titleText.setColour(juce::Label::ColourIds::textColourId, juce::Colours::red);
    titleText.setJustificationType(juce::Justification::centred);
    titleText.setText("MEGASORTEO CRIMINAL RONDADORA", juce::NotificationType::dontSendNotification);
    titleText.setRepaintsOnMouseActivity(false);

    // Button
    addAndMakeVisible(plusButton);
    plusButton.addListener(this);
    plusButton.setImages(true, false, true,
        chapam1, 1.f, juce::Colours::transparentWhite,
        chapam1, 1.f, juce::Colours::transparentWhite,
        chapam1, 1.f, juce::Colours::red.withAlpha(0.8f));

    // Initial Size
    setSize (600, 400);

    // Reset
    updateCounter(0, true);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint(juce::Graphics& g)
{
    //if (needsPaint)
    {
        g.fillAll(juce::Colours::white);
        g.drawImage(fondo, getBounds().toFloat().translated(0, 380), juce::RectanglePlacement::centred);
        g.setColour(juce::Colours::red.withAlpha(0.8f));
        juce::Path p = getGraph();
        g.fillPath(p);
        g.setColour(juce::Colours::black);
        g.drawLine(50, 200, (float)getWidth() - 50, 200, 4.f);
        g.strokePath(p, juce::PathStrokeType(4.f));
        needsPaint = false;
    }
}

void MainComponent::resized()
{
    plusButton.setSize(215, 215);
    plusButton.setCentrePosition(getWidth()/ 2, 500);

    titleText.setSize(getWidth(), 400);
    titleText.setCentrePosition(getWidth() / 2, 100);

    rondaText.setSize(400, 200);
    rondaText.setTopLeftPosition(150, 200);

    rondaValue.setSize(getWidth() / 2, 400);
    rondaValue.setTopLeftPosition(150, 300);

    literText.setSize(400, 200);
    literText.setTopLeftPosition(getWidth() - 150 - 400, 200);

    literValue.setSize(getWidth() / 2, 400);
    literValue.setTopLeftPosition(getWidth() / 2 - 150, 300);
}

void MainComponent::updateCounter(int newValue, bool force)
{
    if (contador != newValue || force)
    {
        contador = newValue >= 0 ? newValue : 0;

        rondaValue.setText(juce::String(contador), juce::NotificationType::dontSendNotification);
        if (rondaValue.getText().length() > 3)
            rondaValue.setFont(juce::Font(300).boldened());
        else
            rondaValue.setFont(juce::Font(400).boldened());
        literValue.setText(juce::String(contador / 3), juce::NotificationType::dontSendNotification);
        if (literValue.getText().length() > 3)
            literValue.setFont(juce::Font(300).boldened());
        else
            literValue.setFont(juce::Font(400).boldened());

        dataPoints.add(DataPoint(contador));
        this->repaint();
    }
}

void MainComponent::buttonClicked(juce::Button* button)
{
    if (button == &plusButton)
    {
        updateCounter(contador + 1);
    }
}

bool MainComponent::keyPressed(const juce::KeyPress& key)
{
    if (key == juce::KeyPress::escapeKey)
    {
        juce::JUCEApplicationBase::quit();
    }
    else if (key == juce::KeyPress::spaceKey)
    {
        plusButton.triggerClick();
        return true;
    }
    else if (key == juce::KeyPress::backspaceKey)
    {
        updateCounter(contador - 1);
        return true;
    }
    else if (key.getTextCharacter() == 'g')
    {
        dataPoints.clear();
        this->repaint();
        return true;
    }
    else if (key.getTextCharacter() == 'r')
    {
        dataPoints.clear();
        updateCounter(0);
        return true;
    }
    else if (key.getTextCharacter() == 't')
    {
        if (isTimerRunning())
            stopTimer();
        else
            startTimer(5000);
    }
    else if (key == sKey)
    {
        updateCounter(0);
        return true;
    }
    else if (key.getTextCharacter() >= '0' && key.getTextCharacter() <= '9')
    {
        if (sKey.isCurrentlyDown())
        {
            int newValuePressed = static_cast<int>(key.getTextCharacter() - '0');
            updateCounter(contador == 0 ? newValuePressed : contador * 10 + newValuePressed);
            return true;
        }
    }
    return false;
}

juce::Path MainComponent::getGraph()
{
    juce::Path path;
    const float minY = (float)getHeight();
    const float maxY = (float)getHeight() - 400;
    const float yRange = maxY - minY;

    if (!dataPoints.isEmpty())
    {
        uint64_t maxValue = 0;
        for (auto& dp : dataPoints)
            if (dp.value > maxValue)
                maxValue = dp.value;

        float xZero = (float)dataPoints.getFirst().time;
        float xRange = (float)dataPoints.getLast().time - xZero;

        path.startNewSubPath(0, minY);
        float x = 0, y = 0;
        for(auto& dp : dataPoints)
        {
            x = &dp == dataPoints.begin() ? 0 : (dp.time - xZero) / xRange * getWidth();
            y = maxValue == 0 ? minY : minY + (yRange * dp.value / (float) maxValue);
            path.lineTo(x, y);
        }
        path.lineTo((float)getWidth(), y);
        path.lineTo((float)getWidth(), minY);
        path.closeSubPath();
    }
   return path;
}

void MainComponent::timerCallback()
{
    updateCounter(contador, true);
}
