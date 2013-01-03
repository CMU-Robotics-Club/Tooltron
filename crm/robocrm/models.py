from django.db import models
from django.contrib.auth.models import User
from django.db.models.signals import post_save


# User Model
class RoboUser(models.Model):
  # Field is required when using profiles 
  user = models.OneToOneField(User)

  # Roboclub RFID Card number
  rfid = models.DecimalField(max_digits=10, decimal_places=0)
  
  # Roboclub Shop Access Permissions
  machines = models.ManyToManyField(Machine)

  # Cell Phone
  cell = models.DecimalField(max_digits=10, decimal_places=0)

  # Class Level
  FRESHMAN = 'FR'
  SOPHOMORE = 'SO'
  JUNIOR = 'JR'
  SENIOR = 'SR'
  MASTERS = 'MS'
  DOCTORAL = 'DR'
  CLASS_LEVEL_CHOICES = (
      (FRESHMAN, 'Freshman'),
      (SOPHOMORE, 'Sophomore'),
      (JUNIOR, 'Junior'),
      (SENIOR, 'Senior'),
      (MASTERs, 'Masters'),
      (DOCTORAL, 'Doctoral'),
  )
  class_level = models.CharField(max_length=2, 
                                 choices=CLASS_LEVEL_CHOICES,
                                 default=FRESHMAN)

  # Graduation Year
  grad_year = models.DecimalField(max_digits=4, decimal_places=0)

  # Primary and Secondary Major/Minors
  major = models.CharField(max_length=20)
  sec_major_one = models.CharField(max_length=20)
  sec_major_two = models.CharField(max_lenght=20)

  #Club Rank
  JUNIOR_MEM = 'JM'
  SENIOR_MEM = 'SM'
  OFFICER = 'OM'
  CLUB_RANK_CHOICES = (
      (JUNIOR_MEM, 'Junior Member'),
      (SENIOR_MEM, 'Senior Member'),
      (OFFICER, 'Officer'),
  )
  club_rank = models.CharField(max_length=2, 
                               choices=CLUB_RANK_CHOICES,
                               default=JUNIOR_MEM)
  
  # Roboclub Transaction Info
  dues_paid = models.DateField()
  tshirt_rec = models.BooleanField(default=False)
  
  # Shop and E-Bench Status
  GOOD = 'GD'
  FIRST_WARN = 'FS'
  SECOND_WARN = 'SD'
  SEM_BAN = 'SB'
  CLUB_BAN = 'CB'
  STATUS_CHOICES = (
      (GOOD, 'Good Standing'),
      (FIRST_WARN, 'First Warning Recieved'),
      (SECOND_WARN, 'Second Warning Recieved'),
      (SEM_BAN, 'Semester Ban'),
      (CLUB_BAN, 'Club Ban')
  )
  bench_status = models.CharField(max_length=2,
                                  choices=STATUS_CHOICES,
                                  default=GOOD)
  shop_status = models.CharField(max_length=2,
                                  choices=STATUS_CHOICES,
                                  default=GOOD)
# needed for Django Auth model
def create_roboclub_user(sender, instance, created, **kwargs):
  if created:
      RoboUser.objects.create(user=instance)
post_save.connect(create_roboclub_user, sender=User)


# Machine Model
class Machine(models.Model)
  type = models.CharField(max_length=20)
  id = models.CharField(max_length=10, unique=True)
  maint = models.BooleanField(default=False)
  dstart = models.DateTimeField()
  dend = models.DateTimeField()

# Event Model
class Event(models.Model)
  type = models.CharField(max_length=30)
  tstart = models.DateTimeField()
  tend = models.DateTimeField()
  user = models.ForeignKey('RoboUser')
  succ = models.BooleanField(default=False)
  imgurl = models.URLField()
  machine = models.ForeignKey('Machine')
  project = models.ForeignKey('Project')
  matuse = models.TextField()

# Project Model
class Project(models.Model)
  name = models.CharField(max_length=30)
  primuser = models.Foreignkey('RoboUser')
  users = models.ManyToManyField('RoboUser')
  charge = models.BooleanField(default=False)

# Roboclub Resources Model
class RoboResource
  type = models.CharField(max_length=30)
  id = models.CharField(max_length=20, unique=True)
  checked_out = models.BooleanField(default=False)
  user = models.ForeignKey('RoboUser')
  time_out = models.DateTimeField()
  time_due = models.DateTimeField()
  officer = models.ForeignKey('RoboUser')


