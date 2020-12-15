#pragma once

namespace blueprint
{
    /** The BlueprintGenericEditor is a default AudioProcessorEditor with preinstalled functionality
     *  for working with Blueprint.
     *
     *  It automatically manages a ReactApplicationRoot, registers some native methods
     *  and properties for interfacing with the editor, and provides some helpful
     *  development tools.
     */
    class BlueprintGenericEditor
        : public juce::AudioProcessorEditor
        , public juce::AudioProcessorParameter::Listener
        , public juce::Timer
    {
    public:
        //==============================================================================
        BlueprintGenericEditor (juce::AudioProcessor&, const juce::File&, juce::AudioProcessorValueTreeState* = nullptr);
        ~BlueprintGenericEditor() override;

        //==============================================================================
        /** Implement the AudioProcessorParameter::Listener interface. */
        void parameterValueChanged (int parameterIndex, float newValue) override;
        void parameterGestureChanged (int parameterIndex, bool gestureIsStarting) override;

        //==============================================================================
        /** Override the timer interface. */
        void timerCallback() override;

        //==============================================================================
        /** Override the component interface. */
        void resized() override;
        void paint (juce::Graphics&) override;

    private:
        //==============================================================================
        /** ReactApplicationRoot bundle eval callback functions */
        void beforeBundleEvaluated();
        void afterBundleEvaluated();

        //==============================================================================
        std::shared_ptr<EcmascriptEngine>     engine;
        ReactApplicationRoot                  appRoot;
        AppHarness                            harness;

        juce::File                            bundleFile;
        juce::AudioProcessorValueTreeState*   valueTreeState;

        //==============================================================================
        // The plugin editor holds an array of parameter value readouts which are
        // propagated to the user interface. During parameter value changes on the
        // realtime thread, we capture the values in this array of structs, then at
        // 30Hz propagate the value changes via dispatching events to the jsui.
        struct ParameterReadout {
            std::atomic<float> value = 0.0;
            std::atomic<bool> dirty = false;

            ParameterReadout() = default;

            ParameterReadout(const ParameterReadout& other) {
                value = other.value.load();
                dirty = other.dirty.load();
            }
        };

        std::vector<ParameterReadout> paramReadouts;

        //==============================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BlueprintGenericEditor)
    };

}
