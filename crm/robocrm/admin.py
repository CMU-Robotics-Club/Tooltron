import string
from robocrm.models import Machine, Event, Project, RoboResource
from django.core.mail import send_mail
from django.forms import ModelForm, ValidationError
from django.contrib import admin
from django.contrib.auth.admin import UserAdmin
from django.contrib.auth.models import User
from django.contrib.auth.forms import UserCreationForm
from robocrm.models import RoboUser

class UserProfileInline(admin.StackedInline):
  model = RoboUser
  can_delete = False
  verbose_name_plural = 'profile'
  filter_horizontal = ('machines',)

  def get_fieldsets(self, request, obj=None):
    if obj:
      # change user form
      return super(UserProfileInline, self).get_fieldsets(request, obj)
    else:
      # add user form
      return (
          (None, {'fields':
            ('cell', 'class_level', 'grad_year', 'major', 'sec_major_one',
              'sec_major_two', 'club_rank', 'dues_paid', 'tshirt_rec',
              'bench_status', 'shop_status')
          }),)

def subscribe_to_list(first_name, last_name, email, listname):
  if email == '':
    return

  name = string.strip(first_name + ' ' + last_name)
  if name == '':
    from_addr = email
  else:
    from_addr = '"' + name + '" <' + email + '>'

  to_addr = listname + '-subscribe@lists.andrew.cmu.edu'

  send_mail('', '', from_addr, [to_addr])

class RoboUserCreationForm(ModelForm):
  # This is modelled directly after django.contrib.auth.forms.UserCreationForm 

  error_messages = UserCreationForm.error_messages
  username = UserCreationForm.declared_fields['username']
  Meta = UserCreationForm.Meta

  def clean_username(self):
    username = self.cleaned_data['username']
    try:
      User.objects.get(username=username)
    except User.DoesNotExist:
      return username
    raise ValidationError(self.error_messages['duplicate_username'])

  def save(self, commit=True):
    user = super(RoboUserCreationForm, self).save(commit=False)
    user.set_password('geek6811')
    if commit:
      user.save()
    subscribe_to_list(user.first_name, user.last_name, user.email, 'roboclub-gb')
    return user

class RoboUserAdmin(UserAdmin):
  inlines = (UserProfileInline, )
  add_fieldsets = (
      (None, {'fields': ('username',)}),
      ('Personal info', {
          'fields': ('first_name', 'last_name', 'email')}),
  )
  add_form = RoboUserCreationForm
  list_display = ('username', 'email', 'first_name', 'last_name')
  search_fields = ['username', 'email', 'first_name', 'last_name']


admin.site.unregister(User)
admin.site.register(User, RoboUserAdmin)
admin.site.register(Machine)
admin.site.register(Event)
admin.site.register(Project)
admin.site.register(RoboResource)
