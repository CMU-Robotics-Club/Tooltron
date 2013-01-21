# Create your views here.
from django.http import HttpResponse
from robocrm.models import *

def index(request):
  return HttpResponse("Hello again, world!")

def roboauth(request, rfid_tag, mach_num):
#  return HttpResponse("Hello, world!")
  r = RoboUser.objects.filter(rfid=rfid_tag)
  if r.count() > 0:
    us = r[0]
  else:
    return HttpResponse("0")
  auth_machines = us.machines.filter(id=mach_num)
  if auth_machines.count() > 0 :
    return HttpResponse("1")
  else :
    return HttpResponse("0")

