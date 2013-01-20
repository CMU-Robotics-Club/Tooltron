# Create your views here.
from django.http import HttpResponse

def index(request):
  return HttpResponse("Hello again, world!")

def roboauth(request, rfid_tag, mach_number):
  return HttpResponse("Hello, world!")
  #r = robocrm.objects.filter(rfid=rfid_tag)
  #auth_machines = r.machines.filter(id=mach_number)
  #if auth_machines.count() > 0 :
  #  return HttpResponse("1")
  #else
  #  return HttpResponse("0")

