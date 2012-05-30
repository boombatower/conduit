; Drush Make (http://drupal.org/project/drush_make)
api = 2
core = 7.x

projects[boolean] = 1.0
projects[entity] = 1.0-rc3
projects[entityreference] = 1.0-rc1
projects[field_group_views] = 1.1
projects[field_helper] = 1.1
projects[field_suppress] = 1.0

projects[serial][type] = module
projects[serial][download][type] = git
projects[serial][download][url] = git://github.com/boombatower/serial.git
projects[serial][download][branch] = 7.x-1.x

projects[services][version] = 3.1
projects[services][patch][1355952] = http://drupal.org/files/1355952-drush-make.patch

projects[services_tools] = 3.2
