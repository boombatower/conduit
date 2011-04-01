<?php
/**
 * @file
 * Provide user uid argument handler that imposes conduit access rules.
 */

/**
 * Argument handler to accept a user id and impose conduit access rules.
 */
class views_plugin_argument_validate_user_conduit extends views_plugin_argument_validate_user {
  function validate_argument($argument) {
    if (parent::validate_argument($argument)) {
      return conduit_user_access(user_load($argument));
    }
    return FALSE;
  }
}
