/* Edge Impulse ingestion SDK
 * Copyright (c) 2021 EdgeImpulse Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/* Include ----------------------------------------------------------------- */
#include "ei_run_classifier.h"
#include "ei_classifier_porting.h"
#include "numpy.hpp"

static const float features[] = {
    // copy raw features here (for example from the 'Live classification' page)
    // see https://docs.edgeimpulse.com/docs/running-your-impulse-spresense

-13.1700, 10.7800, -1.3500, -13.1700, 10.7800, -1.3500, -12.6500, 10.4800, -2.6700, -14.0100, 9.2900, -1.9500, -13.3100, 7.1800, -1.5000, -10.9100, 5.8500, -1.4900, -9.1400, 5.2900, -1.5600, -6.7600, 4.9500, -1.1300, -6.7600, 4.9500, -1.1300, -2.5500, 4.3700, -1.8300, 0.6800, 3.5400, -1.4000, 0.7900, 2.7100, 0.1700, 0.7100, 2.7600, 0.7200, 2.7800, 2.4200, 1.1800, 5.6500, 1.6500, 1.5300, 5.6500, 1.6500, 1.5300, 8.5700, 1.5200, 1.2400, 10.3800, 2.4200, 1.0400, 10.1800, 2.8900, 2.0500, 11.1300, 2.9900, 2.5400, 13.6100, 3.1500, 2.2600, 17.4800, 3.0500, 1.8800, 17.4800, 3.0500, 1.8800, 19.9600, 2.9100, 2.3500, 19.9700, 3.1300, 3.0700, 19.7900, 3.7300, 3.5600, 18.2900, 4.0800, 3.6300, 18.8100, 4.8000, 1.5500, 17.5500, 5.0200, -0.0600, 17.5500, 5.0200, -0.0600, 13.9600, 5.0000, 0.3300, 10.8200, 4.9900, 0.7100, 8.7200, 5.3100, 0.1900, 6.9400, 5.4700, -0.3300, 6.4500, 4.2700, 0.2200, 5.0700, 3.0500, 0.3100, 5.0700, 3.0500, 0.3100, 2.3900, 2.0800, 0.6200, -0.1400, 1.4700, 1.6900, -2.4900, 1.3100, 2.2800, -3.8800, 1.8800, 2.2900, -3.6700, 1.3800, 2.8900, -4.4000, 1.2700, 3.1400, -4.4000, 1.2700, 3.1400, -6.1200, 1.4700, 3.1100, -8.4900, 1.6800, 2.8700, -11.7400, 3.1200, 2.5000, -14.3700, 4.9300, 2.2300, -15.7900, 5.8600, 1.1200, -17.0500, 6.5800, 0.1000, -17.0500, 6.5800, 0.1000, -19.1400, 8.4700, -1.3200, -17.9100, 9.3400, -1.6000, -16.1800, 9.5100, -1.2500, -17.3800, 9.9100, -2.6200, -17.2600, 9.3300, -2.7100, -16.3700, 8.2000, -1.2900, -16.3700, 8.2000, -1.2900, -15.3200, 7.4000, -0.6800, -12.7500, 6.9500, -1.9100, -10.5000, 5.7800, -1.2600, -9.0200, 4.2200, 0.4000, -7.5300, 3.9000, 0.0800, -7.5300, 3.9000, 0.0800, -5.2800, 4.0200, -1.0100, -1.7500, 3.4800, -0.9700, 1.8700, 2.6200, -0.3400, 3.2600, 2.8500, -0.1600, 3.7600, 2.7800, 0.5500, 4.2400, 2.4900, 1.2400, 4.2400, 2.4900, 1.2400, 4.7300, 2.2500, 1.3700, 6.1000, 2.2200, 1.1000, 8.4700, 1.4300, 2.2700, 10.6700, 0.8600, 2.9600, 13.0900, 0.8000, 3.1900, 15.0900, 0.9500, 3.5500, 15.0900, 0.9500, 3.5500, 16.2500, 0.7500, 4.2400, 17.8300, 0.4700, 5.3000, 19.9300, -0.2300, 6.1100, 19.9700, -0.3600, 5.7600, 19.5600, 0.0200, 5.4700, 17.8600, 0.9600, 4.6300, 17.8600, 0.9600, 4.6300, 16.5400, 1.4200, 3.4700, 14.9400, 1.4400, 3.3000, 13.0600, 2.0000, 2.9100, 11.6000, 2.4500, 2.5800, 9.3600, 2.8300, 1.5500, 8.2400, 2.5900, 0.4500, 8.2400, 2.5900, 0.4500, 6.9100, 2.7400, -0.2000, 5.0600, 2.2200, 0.1200, 3.2400, 0.9400, -0.0200, 0.1500, 0.7400, -0.7300, -1.8100, 0.9500, -2.0300, -2.8700, 0.6000, -2.1700, -2.8700, 0.6000, -2.1700, -4.5800, -0.5700, -0.6700, -7.4800, -2.0600, 0.6100, -13.3600, -1.9000, 1.1400, -18.9100, -1.3100, 3.0700, -19.9800, -1.4100, 4.4500, -19.9800, -0.4200, 3.9500, -19.9800, -0.4200, 3.9500, -19.9800, 1.7200, 2.6600, -19.6800, 1.9800, 2.5700, -16.7700, 3.4100, 1.3300, -14.8000, 5.6300, 0.9700, -14.8200, 6.5800, -0.4900, -17.1500, 5.1900, -0.4400, -17.1500, 5.1900, -0.4400, -19.1200, 4.6600, -0.7600, -17.0900, 3.8100, -0.8400, -14.5900, 2.4700, 0.3400, -11.8800, 2.6500, 0.0000, -9.1600, 3.3800, -0.5000, -7.4500, 3.8400, -2.5600, -7.4500, 3.8400, -2.5600, -5.2500, 4.6700, -3.1700, -2.0100, 4.5900, -2.5800, 1.7300, 4.7400, -1.4100, 3.3900, 4.2900, -0.5400, 2.2300, 4.1900, -0.0800


};


int raw_feature_get_data(size_t offset, size_t length, float *out_ptr) {
    memcpy(out_ptr, features + offset, length * sizeof(float));
    return 0;
}

extern "C" int ei_main() {
    ei_printf("Edge Impulse standalone inferencing (TI LaunchXL)\n");

    if (sizeof(features) / sizeof(float) != EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE) {
        ei_printf("The size of your 'features' array is not correct. Expected %d items, but had %u\n",
            EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, sizeof(features) / sizeof(float));
        return 1;
    }

    ei_impulse_result_t result = { 0 };

    while (1) {
        // the features are stored into flash, and we don't want to load everything into RAM
        signal_t features_signal;
        features_signal.total_length = sizeof(features) / sizeof(features[0]);
        features_signal.get_data = &raw_feature_get_data;

        // invoke the impulse
        EI_IMPULSE_ERROR res = run_classifier(&features_signal, &result, true);


        ei_printf("run_classifier returned: %d\n", res);

        if (res != 0) return 1;

        ei_printf("Predictions (DSP: %d ms., Classification: %d ms., Anomaly: %d ms.): \n",
            result.timing.dsp, result.timing.classification, result.timing.anomaly);

        // print the predictions
        ei_printf("[");
        for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
            ei_printf("%.5f", result.classification[ix].value);
#if EI_CLASSIFIER_HAS_ANOMALY == 1
            ei_printf(", ");
#else
            if (ix != EI_CLASSIFIER_LABEL_COUNT - 1) {
                ei_printf(", ");
            }
#endif
        }
#if EI_CLASSIFIER_HAS_ANOMALY == 1
        ei_printf("%.3f", result.anomaly);
#endif
        ei_printf("]\n");

        ei_sleep(2000);
    }
}