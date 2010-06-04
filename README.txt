##Summary

The Availability calendar Module adds a CCK widget named Availability Calendar: Date, that is a compound field that has a From Date, To Date, and Status.

You can use this on any content type. When you add this to your node, you will have a Preview Calendar, that automatically updates as you update your CCK field.

You MUST enable the Availability Calendar: Calendar block on your node, or the Availability calendar will display. This has also been tested with Panels, and should work accordingly.

##How It works

At the time your node loads, we take the output of your fields and create a javascript object named Drupal.availabilityCalendar that matches dates to the calendar id's

Jquery then runs this object against the rendered calendar, and then changes out the css classes, and thusly allows you to have a calendar with 'available' and 'unavailable' classes that you can play with.

this module has been made to be fully theme-able, and you have a lot of control over the output.

##limitations

1. Limited views Integration, DOES NOT INTEGRATE WITH THE CALENDAR MODULE AT ALL

2. Only One CCK Field should be applied to a content type at one time.

##Road Map

1. Work through integration with the calendar module, if at all possible

2. Allow multiple calendars to render on the page at the same time.