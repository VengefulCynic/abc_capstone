package com.capstone.accelerationtester;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.util.Log;

import java.io.*; 
import java.util.Timer;
import java.util.TimerTask;

public class MainActivity extends Activity { 
    private static final String TAG = MainActivity.class.getSimpleName() + " ";
    private Timer timer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        final TextView answerLabel = (TextView) findViewById(R.id.textView1);

        Button readacceleration = (Button) findViewById(R.id.readacceleration); 
        readacceleration.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                Log.i(TAG, "onCreate readacceleration onClick+");

                String X_AXIS = "/sys/bus/i2c/devices/1-0018/x_axis";
                String Y_AXIS = "/sys/bus/i2c/devices/1-0018/y_axis";
                String Z_AXIS = "/sys/bus/i2c/devices/1-0018/z_axis";

               

                    String x = readFile("/sys/bus/i2c/devices/1-0018/x_axis");
                    String y = readFile("/sys/bus/i2c/devices/1-0018/y_axis");
                    String z = readFile("/sys/bus/i2c/devices/1-0018/z_axis");
                

                    Log.i(TAG, "onCreate readacceleration onClick+ x=" + x + ", y=" + y + ", z=" + z);

                    /*FileInputStream xin = new FileInputStream(X_AXIS);
                    byte[] bytesx = new byte[2];
                    double raw_x = xin.read(bytesx, 0, 2);
                    xin.close();
                    
                    FileInputStream yin = new FileInputStream(Y_AXIS);
                    byte[] bytesy = new byte[2];
                    double raw_y = yin.read(bytesy, 0, 2);
                    yin.close();

                    FileInputStream zin = new FileInputStream(Z_AXIS);
                    byte[] bytesz = new byte[2];
                    double raw_z = zin.read(bytesz, 0, 2);
                    zin.close();

                    Log.i(TAG, "onCreate readacceleration- x=" + raw_x + ", y=" + raw_y + ", z=" + raw_z); */

            }
        });

        Button setsamplerate = (Button) findViewById(R.id.setsamplerate); 
        setsamplerate.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                Log.i(TAG, "onCreate setsamplerate+");
                
            }
        });

        Button startmonitor = (Button) findViewById(R.id.startmonitor); 
        startmonitor.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                Log.i(TAG, "onCreate startmonitor+");
            }
        });

        Button stopmonitor = (Button) findViewById(R.id.stopmonitor); 
        stopmonitor.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                Log.i(TAG, "onCreate stopmonitor+");
            }
        });
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    public String readFile(String path) {
        try {
            File file = new File(path); 
            BufferedReader br = new BufferedReader(new FileReader(file)); 
            String st; 
            while ((st = br.readLine()) != null) {
                //Log.i(TAG, " val = " + st);
            } 
            return st;
        }
        catch(IOException ie)
        {
            Log.i(TAG, "Error "+ ie);
        }
        return null;
    }
}
