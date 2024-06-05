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
    rondaText.setColour(juce::Label::ColourIds::textColourId, juce::Colours::black);
    rondaText.setJustificationType(juce::Justification::centredLeft);
    rondaText.setRepaintsOnMouseActivity(false);

    // Ronda Value
    addAndMakeVisible(rondaValue);
    rondaValue.setColour(juce::Label::ColourIds::textColourId, juce::Colours::red);
    rondaValue.setJustificationType(juce::Justification::topLeft);
    rondaValue.setRepaintsOnMouseActivity(false);

    // Litros Text
    addAndMakeVisible(literText);
    literText.setText("Litros Totales", juce::NotificationType::dontSendNotification);
    literText.setColour(juce::Label::ColourIds::textColourId, juce::Colours::black);
    literText.setJustificationType(juce::Justification::centredRight);
    literText.setRepaintsOnMouseActivity(false);

    // Litros Value
    addAndMakeVisible(literValue);
    literValue.setColour(juce::Label::ColourIds::textColourId, juce::Colours::red);
    literValue.setJustificationType(juce::Justification::topRight);
    literValue.setRepaintsOnMouseActivity(false);

    // Title Text
    addAndMakeVisible(titleText);
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

    // Graph
    addAndMakeVisible(graph);

    // Initial Size
    setSize (1280, 720);

    // Reset
    updateCounter(0, true);
    startTimer(5000);
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
        g.drawImage(fondo, getBounds().toFloat().translated(0, getHeight() * JUCE_LIVE_CONSTANT(0.36f)), juce::RectanglePlacement::centred);
        g.setColour(juce::Colours::black);
        g.drawLine(margin, yLineValue, (float)getWidth() - margin, yLineValue, 4.f);
    }
}

void MainComponent::resized()
{
    auto b = getBounds();

    yLineValue = getHeight() * 0.2f;

    titleText.setBounds(b.removeFromTop((int)(getHeight() * 0.2f)).toNearestInt());
    auto titleFont = juce::Font("Arial", "Regular", (float)proportionOfWidth(0.05f)).boldened();
    titleText.setFont(titleFont);

    graph.setBounds(b.removeFromBottom((int)(getHeight() * 0.2f)).toNearestInt());

    b.removeFromBottom((int)(getHeight() * 0.05f));

    plusButton.setSize(215, 215);
    plusButton.setCentrePosition(getWidth()/ 2, (getHeight() - 100)/ 2 );

    b.reduce((int)margin, (int)(margin / 2));
    auto left = b.removeFromLeft(getWidth() / 2);
    auto right = b;

    auto textFont = juce::Font("Arial", "Regular", (float)left.proportionOfWidth(JUCE_LIVE_CONSTANT(0.07f)));

    auto topProportion = JUCE_LIVE_CONSTANT(0.2f);
    rondaText.setBounds(left.removeFromTop((int)(b.getHeight() * topProportion)).toNearestInt());
    literText.setBounds(right.removeFromTop((int)(b.getHeight() * topProportion)).toNearestInt());
    rondaText.setFont(textFont);
    literText.setFont(textFont);


    rondaValue.setBounds(left);
    literValue.setBounds(right);

    updateValueFont();
}

void MainComponent::updateValueFont()
{
    auto numMaxChars = juce::jmax(rondaValue.getText().length(), literValue.getText().length());

    auto valueProportion = JUCE_LIVE_CONSTANT(0.5f);
    if(numMaxChars == 2)
        valueProportion = JUCE_LIVE_CONSTANT(0.47f);
    else if(numMaxChars == 3)
        valueProportion = JUCE_LIVE_CONSTANT(0.37f);
    else if(numMaxChars >= 4)
        valueProportion = JUCE_LIVE_CONSTANT(0.32f);
    auto valueFont = juce::Font("Arial", "Regular", (float)rondaValue.proportionOfWidth(valueProportion)).boldened();

    rondaValue.setFont(valueFont);
    literValue.setFont(valueFont);
}

void MainComponent::updateCounter(int newValue, bool force)
{
    if (contador != newValue || force)
    {
        contador = newValue >= 0 ? newValue : 0;

        rondaValue.setText(juce::String(contador), juce::NotificationType::dontSendNotification);
        literValue.setText(juce::String(contador / 3), juce::NotificationType::dontSendNotification);

        updateValueFont();
        graph.addValue(newValue);

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
        graph.setVisible(!graph.isVisible());
        return true;
    }
    else if (key.getTextCharacter() == 'r')
    {
        graph.clear();
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

void MainComponent::timerCallback()
{
    updateCounter(contador, true);
}
