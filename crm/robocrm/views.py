# Create your views here.

def roboauth(request, rfid_tag, mach_number):
  r = robocrm.objects.filter(rfid=rfid_tag)
  auth_machines = r.machines.filter(id=mach_number)
  if auth_machines.count() > 0 :
    return HttpResponse("1")
  else
    return HttpResponse("0")

