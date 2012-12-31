from django.db import models

# User Model
class RoboUser(AbstractBaseUser):
  andrewID = models.CharField(max_length=8, unique=True, db_index=True)
  



  USERNAME_FIELD = 'andrewID'
  
