# Ledplugin
Attach a led plugin to the mbot

The plugin is used to attach a LED to a mbot and control it's parameters.

We can 

1)Change the color of the LED.

2)Adjust the duraion of the LED flash time.

3)Make the LED appear brighter.

4)Make the LED appear dim.

5)Send a message when a visual object is identified.

First we need to create a LED model.Place this under models folder with a led_sensor model.

<?xml version="1.0" ?>
<sdf version="1.6">
  <model name="led_sensor">
    
    <link name='link'>
        <light name='lamp' type='spot'>
          <pose>0 0 0.0 0 -1.7 0</pose>
          <attenuation>
            <range>2</range>
            <linear>0.025</linear>
          </attenuation>
          <diffuse>0.5 1 1 1</diffuse>
          <specular>1 1 1 1</specular>
          <spot>
            <inner_angle>0.3</inner_angle>
            <outer_angle>0.35</outer_angle>
            <falloff>1</falloff>
          </spot>
          <direction>0 0 -1</direction>
        </light>
        <visual name='lamp'>
          <geometry>
            <sphere>
              <radius>0.025</radius>
            </sphere>
          </geometry>
          <transparency>0</transparency>
          <material>
            <ambient>0.5 1 1 1</ambient>
            <diffuse>0.5 1 1 1</diffuse>
            <specular>1 1 1 1</specular>
            <emissive>0.5 1 1 1</emissive>
          </material>
        </visual>
      
      <plugin name='light_control' filename='libLedPlugin.so'>
        <enable>true</enable>
        <light>
          <id>link/lamp</id>
          <duration>0.5</duration>
          <interval>1</interval>
          <color>1 1 1</color>
          <enable>true</enable>
        </light>
      </plugin>
      </link>
  </model>
</sdf>

Add this model to the tankbot by

 
     <include>
      <uri>model://led_sensor</uri>
      <pose>
        -0.1 0.0 0.25
        0.0 0.0 0.0
      </pose>
    </include>
    <joint name="led_joint" type="fixed">
      <child>led_sensor::link</child>
      <parent>chassis</parent>
    </joint>


    
   The methods
    
   flash() -> Used to increase the brightness of the LED
   
   dim() -> Used to decrease the brightness of the LED
   
   color -> This is set by the constructor of the class.(White is set as default in the code).
    
   Depeneding on the object detected the color can be specifed as a triplet of RGB values(R,G,B) or as specific color words(eg White,Red etc).
    
    
