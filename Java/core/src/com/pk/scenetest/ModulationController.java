package com.pk.scenetest;

import com.badlogic.gdx.ApplicationAdapter;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.audio.Sound;
import com.badlogic.gdx.graphics.GL20;


import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;

public class ModulationController extends ApplicationAdapter{
    private int numberOfFrames = 0;
    int i=0;
    ArrayList<Sound> sounds = new ArrayList<>();
    ArrayList<Integer> numbers;

    @Override
    public void create() {
        sounds.add(Gdx.audio.newSound(Gdx.files.internal("sample_100Hz_1spike.wav")));
        sounds.add(Gdx.audio.newSound(Gdx.files.internal("sample_50Hz_1spike.wav")));

        numbers = new ArrayList<>(Arrays.asList(0,1,2,3,4,5,6,7,8,9,10,11,12,13));
        Collections.shuffle(numbers);
        Gdx.app.error("Numbers:", numbers.toString());
    }

    @Override
    public void render() {

        Gdx.gl.glClearColor(0, 0, 0, 1);
        Gdx.gl.glClear(GL20.GL_COLOR_BUFFER_BIT);

        int frameInterval = 3;

        if (numberOfFrames % frameInterval == 0 && numberOfFrames <= frameInterval * numbers.size() && numberOfFrames != 0) {
            float minVolume = 0.4f;
            float maxVolume = 1.0f;
            float resolution = 6;

            int number = numbers.get(i);
            int waveIndex = 0;
            if (number > 6) {
                waveIndex = 1;
            }
            float volume = minVolume + (number % 7) * ((maxVolume - minVolume) / resolution);
            sounds.get(waveIndex).play(volume);
            i++;
        }
        numberOfFrames ++;
    }
}