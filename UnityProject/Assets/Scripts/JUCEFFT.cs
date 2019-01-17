using UnityEngine;
using UnityEngine.Audio;
using System.Runtime.InteropServices;

public class JUCEFFT : MonoBehaviour
{
	[DllImport("audioplugin_JUCEFFTForUnity")]
    private static extern void GetFFTData(float[] data);

	private static readonly int FFT_SIZE = 2048;
    private float[] fftResult = new float[FFT_SIZE];

    void Update()
    {
        GetFFTData(fftResult);

		float step = 0.025f;
		float scale = 1f;

        for (var i = 0; i < FFT_SIZE / 2; i++) {
            float s = fftResult[i] * scale;
            Debug.DrawLine(new Vector3(i * step, 0, 0), new Vector3(i * step, s * scale, 0), Color.magenta);
        }
    }
}
