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
                    
                String x = readFile("/sys/bus/i2c/devices/1-0018/x_axis");
                String y = readFile("/sys/bus/i2c/devices/1-0018/y_axis");
                String z = readFile("/sys/bus/i2c/devices/1-0018/z_axis");
                Log.i(TAG, "onCreate readacceleration- x=" + x + ", y=" + y + ", z=" + z);
                    
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
        //Get the text file
        File file = new File(path);

        //Read text from file
        StringBuilder text = new StringBuilder();

        try {
            BufferedReader br = new BufferedReader(new FileReader(file));
            String line;

            while ((line = br.readLine()) != null) {
                text.append(line);
                text.append('\n');
            }
            br.close();
        }
        catch (IOException e) {
            //You'll need to add proper error handling here
            Log.i(TAG, "Error "+ e);
        }
        return text.toString();
    }

    
}
