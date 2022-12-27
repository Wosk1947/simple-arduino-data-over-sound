package com.pk.scenetest;

import com.badlogic.gdx.ApplicationAdapter;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.audio.Sound;
import com.badlogic.gdx.graphics.GL20;


import java.util.ArrayList;
import java.util.Arrays;

public class SceneTest extends ApplicationAdapter{
    private int numberOfFrames = 0;
    int i=0;

    ArrayList<Sound> sounds = new ArrayList<>();

    @Override
    public void create() {
        sounds.add(Gdx.audio.newSound(Gdx.files.internal("sample_100Hz_1spike.wav")));
        sounds.add(Gdx.audio.newSound(Gdx.files.internal("sample_200Hz_1spike.wav")));
        sounds.add(Gdx.audio.newSound(Gdx.files.internal("sample_500Hz_1spike.wav")));
        sounds.add(Gdx.audio.newSound(Gdx.files.internal("sample_5000Hz.wav")));
    }

    @Override
    public void render() {

        Gdx.gl.glClearColor(0, 0, 0, 1);
        Gdx.gl.glClear(GL20.GL_COLOR_BUFFER_BIT);

        ArrayList<Integer> numbers = new ArrayList<>(Arrays.asList(5, 0, 1, 4, 2, 6, 3));

        int frameInterval = 3;

        if (numberOfFrames % frameInterval == 0 && numberOfFrames <= frameInterval * numbers.size() && numberOfFrames != 0) {
            float minVolume = 0.3f;
            float maxVolume = 1.0f;
            float resolution = 6;

            int number = numbers.get(i);
            float volume = minVolume + number * ((maxVolume - minVolume) / resolution);
            sounds.get(0).play(volume);
            i++;
        }
        numberOfFrames ++;
    }
}