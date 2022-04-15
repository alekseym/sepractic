using System;
using System.IO;
using System.Net.NetworkInformation;
using System.Threading;

class MainClass
{
    private const string Path = "text_utility.txt";
    private const string AdapterName = "Беспроводная сеть";

    public static void saveCurentTraficConsumprion(string AdapterName, string fileName) {
        if (!NetworkInterface.GetIsNetworkAvailable())
            return;

        NetworkInterface[] interfaces = NetworkInterface.GetAllNetworkInterfaces();

        StreamWriter sw;
        if (!File.Exists(fileName)) {
            // pass the filepath and filename to the StreamWriter Constructor
            sw = File.CreateText(fileName);
        }
        else  {
            sw = File.AppendText(fileName);
        }
        foreach (NetworkInterface ni in interfaces)  {
                // write a line of text: "Bytes Sent: ", and then "Bytes Received: "
                if (ni.Name == AdapterName)
                    sw.WriteLine("{0} {1}", ni.GetIPv4Statistics().BytesSent, ni.GetIPv4Statistics().BytesReceived);
        }
        // close the file stream
        sw.Close();
    }

    static void Main()    {
        for (int i = 0; i < 10; i++) {
            Console.WriteLine(i);
            saveCurentTraficConsumprion(AdapterName, Path);
            Thread.Sleep(10000);
        }
    }
}
