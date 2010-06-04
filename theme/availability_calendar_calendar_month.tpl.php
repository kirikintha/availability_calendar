<?php
/**
 * @param object $month_id
 * @param object $month_name
 * @param object $calendar
 */
?>
<div id="calendar-month-<?php print $month_id; ?>" class="availability-calendar-month" >
	
  <h2 class="calendar-month-title"><?php print $month_name; ?></h2>

  <?php print $calendar; ?>
	
</div>