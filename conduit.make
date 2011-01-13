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

projects[inputstream][type] = module
projects[inputstream][download][type] = git
projects[inputstream][download][revision] = 7.x-1.x
projects[inputstream][download][url] = git://github.com/hugowetterberg/inputstream.git

projects[references] = 2

projects[serial][type] = module
projects[serial][download][type] = git
projects[serial][download][url] = git://github.com/solotandem/serial.git

projects[services][type] = module
projects[services][download][type] = git
projects[services][download][revision] = 7.x-3.x
projects[services][download][url] = git://github.com/kylebrowning/services.git

projects[views] = 3
