Full Plugin

This project displays to the user an onscreen keyboard and a series of dropdown menus and sliders that determine what the synthesiser will sound like.  Sound is produced from the MIDI data collected from the onscreen keyboard and a chosen oscillator wave (sine, sawtooth, or square).  The user can make changes including
   - select the oscillator waveform
   - set the cutoff frequency of the oscillator's ladder filter
   - set the LFO frequency
   - set the component for the LFO to control (cutoff frequency, gain, or ADSR)
   - add a highpass or lowpass filter (with pre-defined coefficents)

This project uses the JUCE code structure for a plugin (with additional classes defined in separate header/.cpp files) and is designed to be run as a standalone plugin.
