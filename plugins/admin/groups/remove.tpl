<div style="font-size:18px;">
  <img alt="" src="images/button_cancel.png" align=top>&nbsp;{t}Warning{/t}
</div>
<p>
  {$info}
  {t}This may be a primary user group. Please double check if you really want to do this since there is no way for GOsa to get your data back.{/t}
</p>

<p>
  {t}So - if you're sure - press 'Delete' to continue or 'Cancel' to abort.{/t}
</p>

<p class="plugbottom">

  {if $multiple}
  <input type=submit name="delete_multiple_groups_confirm" value="{t}Delete{/t}">
  &nbsp;
  <input type=submit name="delete_multiple_group_cancel" value="{t}Cancel{/t}">
  {else}
  <input type=submit name="delete_group_confirm" value="{t}Delete{/t}">
  &nbsp;
  <input type=submit name="delete_cancel" value="{t}Cancel{/t}">
  {/if}


</p>

