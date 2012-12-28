from django.db import models

# User Model
class User(models.model):
  andrewID = models.CharField(max_length=8)
  first_name = models.CharField(max_length=30)
  last_name = models.CharField(max_length=30)
  
