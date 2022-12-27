const int rawValuesArraySize = 16;
float lastRawValues[rawValuesArraySize];
float intervals[] = {20,30,40,50,65,75,85,100};

int rawValueThreshold = 10;

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

void printArray(int* array, int size) {
    for (int i = 0; i < size; i++) {
        Serial.print(array[i]);
        Serial.print(F(" "));
    }
    Serial.println();
}

int getNumber(float* array, int size, float value) {
    for (int i = 0; i <= size - 2; i++) {
       if (value >= array[i] && value < array[i+1]) {
          return i;
       }
    }
    return -1;
}

void loop() {
    int rawValue = analogRead(A1);
    if (rawValue > rawValueThreshold) {
        addValueToArray(lastRawValues, rawValuesArraySize, rawValue);
        applyLowPassFilter(lastRawValues, rawValuesArraySize);
        float currentMaximumAlt = findMaximumOnInterval(lastRawValues, rawValuesArraySize, 0, rawValuesArraySize-2);
        if (currentMaximumAlt != -1) {
          Serial.print(getNumber(intervals, 9, currentMaximumAlt));
          Serial.print(F(" "));     
        }
    }  
}


