# MonolithUI
## About the project
### Description: 
MonolithUI consists of various novel UI elements implemented in Unreal Engine using its Slate and UMG API.

### Future Plans:
#### Implemented:
* Catenary Widget
#### To Do:
* Mesh/Actor Widgets
* Particle Widget
* Knob Widget
* Slider Widget
* Aspect Ratio Widget
* Overlapping Progress Bars 

## Catenary Widget
### Description:
Renders Catenaries, the natural shape a string makes when hung from two points at rest, in real time. 

https://github.com/user-attachments/assets/40c04292-e148-4472-a61b-29578f315ad3

Designers can place a Catenary Widget in the UMG editor like so.

![image](https://github.com/user-attachments/assets/a9d96404-58ed-4810-822e-b6b5c539d8eb)

Then, can instantiate and set various properties of each Catenary in the details panel:

![image](https://github.com/user-attachments/assets/df332408-d106-4284-ab51-5ca3b46ff44e)

In the Point Connection Schema, you can define whether or not one point of the Catenary is static (Schema Type 'Open') or follows a widget as shown in the video (Schema Type 'Widget') so long as you provide a valid name of the widget to follow.
![image](https://github.com/user-attachments/assets/6a155088-2529-458e-9de2-89e7c8eaee4c)


### Future Plans:
* Change parent class to CanvasPanel and rename it to Catenary Panel
* Create 'Catenary Slot' for child widgets to more easily access positional data and for consistency in Coordinate System.
* Rename Schema type 'Open' to 'Static'
