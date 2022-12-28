const int rawValuesArraySize = 32;
float lastRawValues[rawValuesArraySize];
int amplitudeIntervals[] = {30,40,52,64,74,84,94,110};
int wavelengthIntervals[] = {8,12,20};
int rawValueThreshold = 10;

float currentMaximum = 0.0f;
int currentMaximumWidth = 0;

void setup() {
    Serial.begin(9600);
    pinMode(A1, INPUT);
    for (int i = 0; i < rawValuesArraySize; i++){
        lastRawValues[i] = 0;
    }
}

void addValueToArray(float* array, int size, int value) {
    for (int i = 1; i < size; i++) {
        array[i-1] = array[i];
    }
    array[size-1] = (float)value;
}

void applyLowPassFilter(float* array, int size) {
    float alpha = 0.5;
    if (array[size-2] != 0) {
        array[size-1] = alpha * array[size-2] + (1 - alpha) * array[size-1]; 
    }
}

float findMaximumOnInterval(float* array, int size, int start, int interval) {
    float maximum = -1;
    int maxIndex = 0;
    int ascending = 0;
    int descending = 0;
    for (int i = start; i <= start + interval / 2; i++) {
        if (array[i] != 0) {
            if (array[i] < array[i+1]) {
                ascending++;
            }
        }
    }
    for (int i = start + interval / 2 + 1; i <= start + interval; i++) {
        if (array[i] != 0) {
            if (array[i] > array[i+1]) {
                descending++;
            }
        }
    }
    float threshold = 0.7;
    if (ascending > threshold * (interval / 2) && descending > threshold * (interval / 2)) {
        maxIndex = start + interval / 2;
        maximum = array[maxIndex];
        for (int i = 0; i <= start + interval; i++) {  
            array[i] = 0;
        }
    }
    return maximum;
}

void findMaximumAndWidthOnInterval(float* array, int size) {
    currentMaximum = -1;
    currentMaximumWidth = 0;
    int maxIndex = -1;
    int ascending = 0;
    int descending = 0;
    int start = -1;
    int width = 0;
    float halfHeight = 0;
    bool found = false;
    for (int i = start; i < size; i++) {
        if (array[i] == 0) {
            continue;
        } 
        if (array[i] < array[i+1]) {
            if (start == -1){
                start = i;
            }
            ascending++;
            width++;
        }
        if (array[i] > array[i+1]) {
            if (ascending > 0){
                if (maxIndex == -1){
                    maxIndex = i;
                }
                descending++;
                width++;
            }
        }
        int pointsCountThreshold = 4; //Original = 2
        float pointsBalanceThreshold = 0.6; // Original = 0.7
        if (ascending > pointsCountThreshold && descending > pointsCountThreshold && descending > pointsBalanceThreshold * ascending){
            found = true;
            break;
        }
    }
    if (found) {
        currentMaximum = array[maxIndex];
        currentMaximumWidth = width;
        Serial.print(getNumber(amplitudeIntervals, 8, wavelengthIntervals, 3, currentMaximum, currentMaximumWidth));
        Serial.print(F(" "));
        for (int i = 0; i <= maxIndex; i++) {  
            array[i] = 0;
        }
    }
}

void printArray(int* array, int size) {
    for (int i = 0; i < size; i++) {
        Serial.print(array[i]);
        Serial.print(F(" "));
    }
    Serial.println();
}

int getNumber(int* ampIntervals, int size, int* waveIntervals, int size2, float amplitude, int wavelength) {
    for (int i = 0; i <= size - 2; i++) {
       if (amplitude >= ampIntervals[i] && amplitude < ampIntervals[i+1]) {
          for (int j = 0; j < size2; j++){
              if (wavelength >= waveIntervals[j] && wavelength < waveIntervals[j+1]) {
                  return j * (size-1) + i;
              }   
          }
       }
    }
    return -1;
}

void loop() {
    int rawValue = analogRead(A1);
    if (rawValue > rawValueThreshold) {
        addValueToArray(lastRawValues, rawValuesArraySize, rawValue);
        applyLowPassFilter(lastRawValues, rawValuesArraySize);
        findMaximumAndWidthOnInterval(lastRawValues, rawValuesArraySize);
    }  
}


