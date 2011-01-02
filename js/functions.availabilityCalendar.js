/**
 * Property Availability JS
 * Creates an availability calendar JS object, so we can apply calendars to the page via javascript.
 */
if (Drupal.jsEnabled) {
  //Make sure we have valid objects at all time
  Drupal.settings.availabilityCalendar = Drupal.settings.availabilityCalendar || {};
  //Document Ready
  $(document).ready( function() {
    //Update and rebind elements onLoad.
    Drupal.behaviors.availabilityCalendar._updateCalendar();
    Drupal.behaviors.availabilityCalendar._rebindElements();
    //Bind ajaxComplete when we are on a CCK field.
    $(document).bind('ajaxComplete', function() {
      //When any multiple cck elements are created, we have to rebind the CCK widgets so the new elements work with the live calendar preview.
      Drupal.behaviors.availabilityCalendar._rebindElements();
    });
  //end document ready
  });
  //Rebind CCK elements when we add to multiple.
  Drupal.behaviors.availabilityCalendar._rebindElements = function() {
    //When switching through availability via the select, update the calendar below
    $('div.availability-calendar-date-widget select.form-select').bind('change', function () {
      //reset the property Availability object
      Drupal.settings.availabilityCalendar = {}
      Drupal.behaviors.availabilityCalendar._resetCalendarPreview();
    });
    //When we blur the field, we need to recalculate the preview
    $('div.availability-calendar-date-widget input.availability-calendar-date-field-from, div.availability-calendar-date-widget input.availability-calendar-date-field-to, ').bind('change', function () {
      //We have to sanity check here, that this date does not conflict with it's corresponding to/from date
      var inputs = $(this).parent('div').parent('div').children('div').children('input');
      if ( inputs[0].value && inputs[1].value ) {
        //Get a valid Start Date for this date set
        var start = inputs[0].value;
        startDate = new Date( start );
        //Get a Valid End Date for this date set
        var end = inputs[1].value;
        endDate = new Date( end );
        //If you have not entered in a valid set of dates
        if ( start >= end ) {
          alert( Drupal.t('Sorry, but you must enter an To Date that is greater than the From Date.') );
          $(this).val('');
        } else {
          //If you have entered in a valid set of dates, reset that preview!
          Drupal.settings.availabilityCalendar = {}
          Drupal.behaviors.availabilityCalendar._resetCalendarPreview();
        }
      }
    });
  }
  //Reset Calendar Preview, live update of your Availability
  Drupal.behaviors.availabilityCalendar._resetCalendarPreview = function() {
    $('div.availability-calendar-date-widget').each( function () {
      var inputs = $(this).children('div').children('input');
      var status = $(this).children('div').children('select').val();
      Drupal.behaviors.availabilityCalendar._updateAvailability( inputs, status );
    });
  }
  //Update the Availability Object, then update the Preview Calendar
  Drupal.behaviors.availabilityCalendar._updateAvailability = function(inputs, status) {
    //Get a valid Start Date
    var start = inputs[0].value;
    startDate = new Date( start );
    //Get a Valid End Date
    var end = inputs[1].value;
    endDate = new Date( end );
    var daysApart = Math.abs( Math.round( ( startDate - endDate ) / 86400000 ) );
    if ( !isNaN( daysApart ) ) { //If we have a number then we can recalculate the days in between and reset the Availability Object
      for ( var i = 0; i <= daysApart; i++ ) {
        var midDay = '';
        //make sure we add the mid-day-first class where necessary
        if ( i == 0 || i == daysApart ) {
          midDay = ' mid-day-first';
        }
        //make sure we add the mid-day-last class where necessary
        if ( i == daysApart ) {
          midDay = ' mid-day-last';
        }
        //Create dates.
        newDate = new Date( startDate );
        newDate.setDate( startDate.getDate() + i );
        //We have to get the month + 1 because it is a zero array, and the rest of the date stuff is fine
        newDate = ( newDate.getMonth() + 1 ) + "-" + newDate.getDate() + "-" + newDate.getFullYear(); 
        //Add the new dates to the propertyAvailability Array, and then send off the changes to updateCalendar
        Drupal.settings.availabilityCalendar[newDate] = status + midDay;
      }
    }
    //After we have everything reset, update that Caendar!
    Drupal.behaviors.availabilityCalendar._updateCalendar();
  }
  //Update the preview calendar with the live information
  Drupal.behaviors.availabilityCalendar._updateCalendar = function() {
    //Set all dates to unavailable first
    $('table.availability-calendar td.calendar-day').removeClass('unavailable').addClass('available');
    for ( day in Drupal.settings.availabilityCalendar ) {
      $('table.availability-calendar td#' + day ).removeClass('available').addClass( Drupal.settings.availabilityCalendar[day] );
    }
  }
  //End JS endabled
}