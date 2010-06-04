/**
 * Property Availability JS - takes an object
 *   Drupal.availabilityCalendar = {
    '01-01-2010' : 'unavailable'
  }
 * 
 */
if (Drupal.jsEnabled) {
  
  //Make sure we have valid objects at all time
  Drupal.availabilityCalendar = Drupal.availabilityCalendar || {};
	
	$(document).ready( function() {
    
    _updateCalendar();
    _rebindElements();
    
    $(document).bind( 'ajaxComplete', function() {
      
      //Drupal.trace( 'Ajax Complete' );
      _rebindElements();
      
    } );
    
    function _rebindElements() {
    
      //When switching through availability via the select, update the calendar below
      $('div.availability-calendar-date-widget select.form-select').bind( 'change', function () {
        
        Drupal.availabilityCalendar = {} //reset the property Availability object
        
        _resetCalendarPreview();
        
      } );
      
      //When we blur the field, we need to recalculate the preview
      $('div.availability-calendar-date-widget input.form-text').bind( 'change', function () {
        
        //We have to sanity check here, that this date does not conflict with it's corresponding to/from date
        
        //Find out which field we are looking
        //Drupal.trace( $(this).parent('div').parent('div').children('div').children('input') );
        
        var inputs = $(this).parent('div').parent('div').children('div').children('input');
        
        if ( inputs[0].value && inputs[1].value ) {
          
          //Get a valid Start Date for this date set
          var start = inputs[0].value;
          startDate = new Date( start );
          
          //Get a Valid End Date for this date set
          var end = inputs[1].value;
          endDate = new Date( end );
          
          if ( start >= end ) { //If you have not entered in a valid set of dates
            
            alert( Drupal.t('Sorry, but you must enter an To Date that is greater than the From Date.') );
            
            $(this).val('');
          
          } else { //If you have entered in a valid set of dates, reset that preview!
          
            Drupal.availabilityCalendar = {} //reset the property Availability object
            
            _resetCalendarPreview();
            
          }
          
        }
        
      } );
    
    }
        
    //Reset Calendar Preview, live update of your Availability
    function _resetCalendarPreview() {
      
      //Drupal.trace( $('div.availability-calendar-date-widget') );
      
      $('div.availability-calendar-date-widget').each( function () {
        
        var inputs = $(this).children('div').children('input');
        var status = $(this).children('div').children('select').val();
        
        _updateAvailability( inputs, status );
      
      } );
      
    }
    
    //Update the Availability Object, then update the Preview Calendar
    function _updateAvailability( inputs, status ) {
      
      //Get a valid Start Date
      var start = inputs[0].value;
      startDate = new Date( start );
      
      //Get a Valid End Date
      var end = inputs[1].value;
      endDate = new Date( end );
      //Drupal.trace( end );
      
      var daysApart = Math.abs( Math.round( ( startDate - endDate ) / 86400000 ) );
      //Drupal.trace( daysApart );
      
      if ( !isNaN( daysApart ) ) { //If we have a number then we can recalculate the days in between and reset the Availability Object
        
        for ( var i = 0; i <= daysApart; i++ ) {
          
          newDate = new Date( startDate );
          newDate.setDate( startDate.getDate() + i );
          //Drupal.trace( newDate );
          newDate = ( newDate.getMonth() + 1 ) + "-" + newDate.getDate() + "-" + newDate.getFullYear(); //We have to get the month + 1 because it is a zero array, and the rest of the date stuff is fine
          //Drupal.trace( newDate );
          Drupal.availabilityCalendar[newDate] = status; //Add the new dates to the propertyAvailability Array, and then send off the changes to updateCalendar
          
        }
        
      }
      
      //Drupal.trace( Drupal.availabilityCalendar );
      _updateCalendar(); //After we have everything reset, update that Caendar!
      
    }
    
    
    //Update the preview calendar with the live information
    function _updateCalendar() {
      
      //Set all dates to unavailable first
      $('table.availability-calendar td.calendar-day').removeClass('unavailable').addClass('available');
  
      for ( day in Drupal.availabilityCalendar ) {
        
        //Drupal.trace( day + Drupal.availabilityCalendar[day] );
        $('table.availability-calendar td#' + day ).removeClass('available').addClass( Drupal.availabilityCalendar[day] );
        
      }
      
    }
    
    //end document ready
    
	} );
  
  //End JS endabled

}