# -*- coding: utf-8 -*-
import datetime
from south.db import db
from south.v2 import SchemaMigration
from django.db import models


class Migration(SchemaMigration):

    def forwards(self, orm):
        # Adding model 'Machine'
        db.create_table('robocrm_machine', (
            ('type', self.gf('django.db.models.fields.CharField')(max_length=20)),
            ('id', self.gf('django.db.models.fields.CharField')(max_length=10, primary_key=True)),
            ('maint', self.gf('django.db.models.fields.BooleanField')(default=False)),
            ('dstart', self.gf('django.db.models.fields.DateTimeField')()),
            ('dend', self.gf('django.db.models.fields.DateTimeField')()),
        ))
        db.send_create_signal('robocrm', ['Machine'])

        # Adding model 'RoboUser'
        db.create_table('robocrm_robouser', (
            ('id', self.gf('django.db.models.fields.AutoField')(primary_key=True)),
            ('user', self.gf('django.db.models.fields.related.OneToOneField')(to=orm['auth.User'], unique=True)),
            ('rfid', self.gf('django.db.models.fields.DecimalField')(max_digits=10, decimal_places=0)),
            ('cell', self.gf('django.db.models.fields.DecimalField')(max_digits=10, decimal_places=0)),
            ('class_level', self.gf('django.db.models.fields.CharField')(default='FR', max_length=2)),
            ('grad_year', self.gf('django.db.models.fields.DecimalField')(max_digits=4, decimal_places=0)),
            ('major', self.gf('django.db.models.fields.CharField')(max_length=20)),
            ('sec_major_one', self.gf('django.db.models.fields.CharField')(max_length=20)),
            ('sec_major_two', self.gf('django.db.models.fields.CharField')(max_length=20)),
            ('club_rank', self.gf('django.db.models.fields.CharField')(default='JM', max_length=2)),
            ('dues_paid', self.gf('django.db.models.fields.DateField')()),
            ('tshirt_rec', self.gf('django.db.models.fields.BooleanField')(default=False)),
            ('bench_status', self.gf('django.db.models.fields.CharField')(default='GD', max_length=2)),
            ('shop_status', self.gf('django.db.models.fields.CharField')(default='GD', max_length=2)),
        ))
        db.send_create_signal('robocrm', ['RoboUser'])

        # Adding M2M table for field machines on 'RoboUser'
        db.create_table('robocrm_robouser_machines', (
            ('id', models.AutoField(verbose_name='ID', primary_key=True, auto_created=True)),
            ('robouser', models.ForeignKey(orm['robocrm.robouser'], null=False)),
            ('machine', models.ForeignKey(orm['robocrm.machine'], null=False))
        ))
        db.create_unique('robocrm_robouser_machines', ['robouser_id', 'machine_id'])

        # Adding model 'Project'
        db.create_table('robocrm_project', (
            ('id', self.gf('django.db.models.fields.AutoField')(primary_key=True)),
            ('name', self.gf('django.db.models.fields.CharField')(max_length=30)),
            ('primuser', self.gf('django.db.models.fields.related.ForeignKey')(related_name='pri+', to=orm['robocrm.RoboUser'])),
            ('charge', self.gf('django.db.models.fields.BooleanField')(default=False)),
        ))
        db.send_create_signal('robocrm', ['Project'])

        # Adding M2M table for field users on 'Project'
        db.create_table('robocrm_project_users', (
            ('id', models.AutoField(verbose_name='ID', primary_key=True, auto_created=True)),
            ('project', models.ForeignKey(orm['robocrm.project'], null=False)),
            ('robouser', models.ForeignKey(orm['robocrm.robouser'], null=False))
        ))
        db.create_unique('robocrm_project_users', ['project_id', 'robouser_id'])

        # Adding model 'RoboResource'
        db.create_table('robocrm_roboresource', (
            ('type', self.gf('django.db.models.fields.CharField')(max_length=30)),
            ('id', self.gf('django.db.models.fields.CharField')(max_length=20, primary_key=True)),
            ('checked_out', self.gf('django.db.models.fields.BooleanField')(default=False)),
            ('user', self.gf('django.db.models.fields.related.ForeignKey')(related_name='u+', to=orm['robocrm.RoboUser'])),
            ('time_out', self.gf('django.db.models.fields.DateTimeField')()),
            ('time_due', self.gf('django.db.models.fields.DateTimeField')()),
            ('officer', self.gf('django.db.models.fields.related.ForeignKey')(related_name='o+', to=orm['robocrm.RoboUser'])),
        ))
        db.send_create_signal('robocrm', ['RoboResource'])

        # Adding model 'Event'
        db.create_table('robocrm_event', (
            ('id', self.gf('django.db.models.fields.AutoField')(primary_key=True)),
            ('type', self.gf('django.db.models.fields.CharField')(max_length=30)),
            ('tstart', self.gf('django.db.models.fields.DateTimeField')()),
            ('tend', self.gf('django.db.models.fields.DateTimeField')()),
            ('user', self.gf('django.db.models.fields.related.ForeignKey')(to=orm['robocrm.RoboUser'])),
            ('succ', self.gf('django.db.models.fields.BooleanField')(default=False)),
            ('imgurl', self.gf('django.db.models.fields.URLField')(max_length=200)),
            ('machine', self.gf('django.db.models.fields.related.ForeignKey')(to=orm['robocrm.Machine'])),
            ('project', self.gf('django.db.models.fields.related.ForeignKey')(to=orm['robocrm.Project'])),
            ('matuse', self.gf('django.db.models.fields.TextField')()),
        ))
        db.send_create_signal('robocrm', ['Event'])


    def backwards(self, orm):
        # Deleting model 'Machine'
        db.delete_table('robocrm_machine')

        # Deleting model 'RoboUser'
        db.delete_table('robocrm_robouser')

        # Removing M2M table for field machines on 'RoboUser'
        db.delete_table('robocrm_robouser_machines')

        # Deleting model 'Project'
        db.delete_table('robocrm_project')

        # Removing M2M table for field users on 'Project'
        db.delete_table('robocrm_project_users')

        # Deleting model 'RoboResource'
        db.delete_table('robocrm_roboresource')

        # Deleting model 'Event'
        db.delete_table('robocrm_event')


    models = {
        'auth.group': {
            'Meta': {'object_name': 'Group'},
            'id': ('django.db.models.fields.AutoField', [], {'primary_key': 'True'}),
            'name': ('django.db.models.fields.CharField', [], {'unique': 'True', 'max_length': '80'}),
            'permissions': ('django.db.models.fields.related.ManyToManyField', [], {'to': "orm['auth.Permission']", 'symmetrical': 'False', 'blank': 'True'})
        },
        'auth.permission': {
            'Meta': {'ordering': "('content_type__app_label', 'content_type__model', 'codename')", 'unique_together': "(('content_type', 'codename'),)", 'object_name': 'Permission'},
            'codename': ('django.db.models.fields.CharField', [], {'max_length': '100'}),
            'content_type': ('django.db.models.fields.related.ForeignKey', [], {'to': "orm['contenttypes.ContentType']"}),
            'id': ('django.db.models.fields.AutoField', [], {'primary_key': 'True'}),
            'name': ('django.db.models.fields.CharField', [], {'max_length': '50'})
        },
        'auth.user': {
            'Meta': {'object_name': 'User'},
            'date_joined': ('django.db.models.fields.DateTimeField', [], {'default': 'datetime.datetime.now'}),
            'email': ('django.db.models.fields.EmailField', [], {'max_length': '75', 'blank': 'True'}),
            'first_name': ('django.db.models.fields.CharField', [], {'max_length': '30', 'blank': 'True'}),
            'groups': ('django.db.models.fields.related.ManyToManyField', [], {'to': "orm['auth.Group']", 'symmetrical': 'False', 'blank': 'True'}),
            'id': ('django.db.models.fields.AutoField', [], {'primary_key': 'True'}),
            'is_active': ('django.db.models.fields.BooleanField', [], {'default': 'True'}),
            'is_staff': ('django.db.models.fields.BooleanField', [], {'default': 'False'}),
            'is_superuser': ('django.db.models.fields.BooleanField', [], {'default': 'False'}),
            'last_login': ('django.db.models.fields.DateTimeField', [], {'default': 'datetime.datetime.now'}),
            'last_name': ('django.db.models.fields.CharField', [], {'max_length': '30', 'blank': 'True'}),
            'password': ('django.db.models.fields.CharField', [], {'max_length': '128'}),
            'user_permissions': ('django.db.models.fields.related.ManyToManyField', [], {'to': "orm['auth.Permission']", 'symmetrical': 'False', 'blank': 'True'}),
            'username': ('django.db.models.fields.CharField', [], {'unique': 'True', 'max_length': '30'})
        },
        'contenttypes.contenttype': {
            'Meta': {'ordering': "('name',)", 'unique_together': "(('app_label', 'model'),)", 'object_name': 'ContentType', 'db_table': "'django_content_type'"},
            'app_label': ('django.db.models.fields.CharField', [], {'max_length': '100'}),
            'id': ('django.db.models.fields.AutoField', [], {'primary_key': 'True'}),
            'model': ('django.db.models.fields.CharField', [], {'max_length': '100'}),
            'name': ('django.db.models.fields.CharField', [], {'max_length': '100'})
        },
        'robocrm.event': {
            'Meta': {'object_name': 'Event'},
            'id': ('django.db.models.fields.AutoField', [], {'primary_key': 'True'}),
            'imgurl': ('django.db.models.fields.URLField', [], {'max_length': '200'}),
            'machine': ('django.db.models.fields.related.ForeignKey', [], {'to': "orm['robocrm.Machine']"}),
            'matuse': ('django.db.models.fields.TextField', [], {}),
            'project': ('django.db.models.fields.related.ForeignKey', [], {'to': "orm['robocrm.Project']"}),
            'succ': ('django.db.models.fields.BooleanField', [], {'default': 'False'}),
            'tend': ('django.db.models.fields.DateTimeField', [], {}),
            'tstart': ('django.db.models.fields.DateTimeField', [], {}),
            'type': ('django.db.models.fields.CharField', [], {'max_length': '30'}),
            'user': ('django.db.models.fields.related.ForeignKey', [], {'to': "orm['robocrm.RoboUser']"})
        },
        'robocrm.machine': {
            'Meta': {'object_name': 'Machine'},
            'dend': ('django.db.models.fields.DateTimeField', [], {}),
            'dstart': ('django.db.models.fields.DateTimeField', [], {}),
            'id': ('django.db.models.fields.CharField', [], {'max_length': '10', 'primary_key': 'True'}),
            'maint': ('django.db.models.fields.BooleanField', [], {'default': 'False'}),
            'type': ('django.db.models.fields.CharField', [], {'max_length': '20'})
        },
        'robocrm.project': {
            'Meta': {'object_name': 'Project'},
            'charge': ('django.db.models.fields.BooleanField', [], {'default': 'False'}),
            'id': ('django.db.models.fields.AutoField', [], {'primary_key': 'True'}),
            'name': ('django.db.models.fields.CharField', [], {'max_length': '30'}),
            'primuser': ('django.db.models.fields.related.ForeignKey', [], {'related_name': "'pri+'", 'to': "orm['robocrm.RoboUser']"}),
            'users': ('django.db.models.fields.related.ManyToManyField', [], {'related_name': "'u+'", 'symmetrical': 'False', 'to': "orm['robocrm.RoboUser']"})
        },
        'robocrm.roboresource': {
            'Meta': {'object_name': 'RoboResource'},
            'checked_out': ('django.db.models.fields.BooleanField', [], {'default': 'False'}),
            'id': ('django.db.models.fields.CharField', [], {'max_length': '20', 'primary_key': 'True'}),
            'officer': ('django.db.models.fields.related.ForeignKey', [], {'related_name': "'o+'", 'to': "orm['robocrm.RoboUser']"}),
            'time_due': ('django.db.models.fields.DateTimeField', [], {}),
            'time_out': ('django.db.models.fields.DateTimeField', [], {}),
            'type': ('django.db.models.fields.CharField', [], {'max_length': '30'}),
            'user': ('django.db.models.fields.related.ForeignKey', [], {'related_name': "'u+'", 'to': "orm['robocrm.RoboUser']"})
        },
        'robocrm.robouser': {
            'Meta': {'object_name': 'RoboUser'},
            'bench_status': ('django.db.models.fields.CharField', [], {'default': "'GD'", 'max_length': '2'}),
            'cell': ('django.db.models.fields.DecimalField', [], {'max_digits': '10', 'decimal_places': '0'}),
            'class_level': ('django.db.models.fields.CharField', [], {'default': "'FR'", 'max_length': '2'}),
            'club_rank': ('django.db.models.fields.CharField', [], {'default': "'JM'", 'max_length': '2'}),
            'dues_paid': ('django.db.models.fields.DateField', [], {}),
            'grad_year': ('django.db.models.fields.DecimalField', [], {'max_digits': '4', 'decimal_places': '0'}),
            'id': ('django.db.models.fields.AutoField', [], {'primary_key': 'True'}),
            'machines': ('django.db.models.fields.related.ManyToManyField', [], {'to': "orm['robocrm.Machine']", 'symmetrical': 'False'}),
            'major': ('django.db.models.fields.CharField', [], {'max_length': '20'}),
            'rfid': ('django.db.models.fields.DecimalField', [], {'max_digits': '10', 'decimal_places': '0'}),
            'sec_major_one': ('django.db.models.fields.CharField', [], {'max_length': '20'}),
            'sec_major_two': ('django.db.models.fields.CharField', [], {'max_length': '20'}),
            'shop_status': ('django.db.models.fields.CharField', [], {'default': "'GD'", 'max_length': '2'}),
            'tshirt_rec': ('django.db.models.fields.BooleanField', [], {'default': 'False'}),
            'user': ('django.db.models.fields.related.OneToOneField', [], {'to': "orm['auth.User']", 'unique': 'True'})
        }
    }

    complete_apps = ['robocrm']