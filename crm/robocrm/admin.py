from robocrm.models import Machine
from robocrm.models import Event
from robocrm.models import Project
from robocrm.models import RoboResource
from django.contrib import admin
from django.contrib.auth.admin import UserAdmin
from django.contrib.auth.models import User
from robocrm.models import RoboUser

class UserProfileInLine(admin.StackedInline):
  model = RoboUser
  can_delete=False
  verbose_name_plural = 'profile'
  filter_horizontal = ('machines',)

class UserAdmin(UserAdmin):
  inlines = (UserProfileInLine, )
  list_display = ('username', 'email', 'first_name', 'last_name')
  search_fields = ['username', 'email', 'first_name', 'last_name']


admin.site.unregister(User)
admin.site.register(User, UserAdmin)
admin.site.register(Machine)
admin.site.register(Event)
admin.site.register(Project)
admin.site.register(RoboResource)

