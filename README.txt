##Summary

The Availability calendar Module adds a CCK widget named Availability Calendar: Date, that is a compound field that has a From Date, To Date, and Status. This Date field is modeled after the Date CCK widget, but IS NOT a Date widget, and as far as I can tell, is not compatible with the Calendar Module.

You can use this on any content type. When you add this to your node, you will have a Preview Calendar, that automatically updates as you update your CCK field. Single, Multiple and Unlimited values are allowed.

NOTE: You MUST enable the Availability Calendar: Calendar block on your node display, or the Availability calendar will NOT display when you View your Node. This has also been tested with Panels, and should work accordingly. No extra configuration is necessary to view the availability calendar in the Node Edit.

RECOMMENDED: It is also Recommended that you use the Vertical Tabs Module, as this allows you to have a cleaner interface when viewing these complex and sometimes numerous entries in one field. My advice is to create a Field Group Named "Availability Calendar" and keep your availability date field in there.

##How It works

At the time your node loads, we take the output of your fields and create a javascript object named Drupal.availabilityCalendar that matches dates to the calendar id's and output that in <script> tags.

Jquery then runs this object against the rendered calendar, and then changes out the css classes, and thusly allows you to have a calendar with 'available' and 'unavailable' classes that you can play with. This is a nice way to not have a lot of extra logic for changing the availability of the calendar, and allows for more complex rendering from Developers.

This module has been made to be fully theme-able, and you have a lot of control over the output. Please check the /theme file for the .tpl of your choice.

##Notes on Compatibility
This module has been tested with the latest release of Drupal, which is 6.17 and the latest version of the Content Module (CCK) 6.26. I can confirm this works on Drupal Core 6.14 and later, but please use the latest version of CCK so you don't run into trouble. I don't see why this wouldn't work with earlier versions of Drupal Core and CCK, but I am officially stating that I can only support you if you have an Up to Date site.

NOTE: This will NEVER be legacy ported to Drupal 5, I am truly sorry, but I don't want to maintain two separate versions of Drupal that are becoming out of date. However, I do pledge that I will start a Drupal 7 branch when D7 becomes more stable, I would prefer to start this when 7.1 comes out, which it will.

##Limitations

1. Limited views Integration, not too much complexity there.

2. DOES NOT INTEGRATE WITH THE CALENDAR MODULE AT ALL

3. Only ONE (1) CCK Field should be applied to a content type at one time. There will be problems if you try and put many fields on one content type.

4. Displays only (1) calendar per node, or page. No support for multiple calendars yet, sorry.

5. Displays a Gregorian Calendar only.

##Dependencies

Content Module (CCK)
Date Popup
Jquery 1.2.6

##Road Map

1. Allow multiple calendars to render on the page at the same time.

2. Better Validation for incorrect user actions, and a backup server side _validate for bad date ranges, in case something gets past the javascript.

3. Choose what date popup to use, or add your own.

4. Change the way the theming works, so we can have a pattern to match.

5. Make the CCK widget have more configuration options.

6. Make the block have configuration options, if needed.

##Contacting the Maintainer of This project

Please feel free to contact me via github or drupal.org. My handle is "kirikintha" it's easy to find me. Support requests should allow between 3-5 business days to reply, and I have to reserve the right to push off new features to the next version. My plan is to have quarterly releases, unless there is an emergency bug that crashes everything, which I will do my best to resolve as quickly as I can.

Thank you very much for using my module, I hope it works for you bug free!