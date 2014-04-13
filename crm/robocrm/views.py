# Create your views here.
from django.http import HttpResponse, Http404
from django.core.exceptions import PermissionDenied, DoesNotExist
from django.contrib.auth import authenticate, login
from robocrm.models import *

def index(request):
  return HttpResponse("Hello again, world!")

def roboauth(request, rfid_tag, mach_num):
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

def roboauthall(request, rfid_tag):
  r = RoboUser.objects.filter(rfid=rfid_tag)
  if r.count() > 0:
    us = r[0]
  else:
    return HttpResponse("0")
  auth = 0
  for mach in us.machines.all():
    auth |= 1 << int(mach.id)
  return HttpResponse(str(auth))

def add_card_event(request):
  if request.method != 'POST':
    raise Http404
  if 'username' in request.POST and 'password' in request.POST:
    user = authenticate(username=request.POST['username'],
        password=request.POST['password'])
    if user is not None and user.is_active:
      login(request, user)

  if not request.user.is_authenticated() \
      or not request.user.has_perm('robocrm.add_event'):
    raise PermissionDenied

  tstart = request.POST['tstart'] # TODO convert to date
  tend = request.POST['tend']
  user_id = request.POST['user_id']
  succ = request.POST['succ'] == '1'
  imgurl = '' # TODO find url based on tstart
  machine_id = int(request.POST['machine_id'])

  try:
    robouser = RoboUser.objects.get(rfid__iexact=user_id)
  except DoesNotExist:
    robouser = None

  try:
    machine = Machine.objects.get(id__exact=machine_id)
  except DoesNotExist:
    machine = None

  ev = Event(type='card',
      tstart=tstart,
      tend=tend,
      user=robouser,
      succ=succ,
      imgurl=imgurl,
      machine=machine,
      project=None,
      matuse='')

  ev.save()

  return HttpResponse()
