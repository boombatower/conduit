; Drush Make (http://drupal.org/project/drush_make)
api = 2

; Drupal core

core = 7.x
projects[drupal] = 7

; Dependencies.

projects[boolean][type] = module
projects[boolean][download][type] = git
projects[boolean][download][url] = git://github.com/solotandem/boolean.git

projects[field_helper] = 1
projects[references] = 2

projects[serial][type] = module
projects[serial][download][type] = git
projects[serial][download][url] = git://github.com/solotandem/serial.git

projects[services] = 3
projects[views] = 3
