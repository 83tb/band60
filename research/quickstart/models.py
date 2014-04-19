from django.db import models
from django.contrib import admin



from taggit.managers import TaggableManager


# Create your models here.

class Person(models.Model):
    name = models.CharField(max_length=180)
    age = models.IntegerField()
    gender = models.CharField(max_length=180, choices=[("male","male"),("female","female")])

class TimeSeries(models.Model):
    when = models.DateTimeField()
    xvalue = models.FloatField(blank=True)
    yvalue = models.FloatField(blank=True)
    zvalue = models.FloatField(blank=True)
    qvalue = models.FloatField(blank=True)
    wvalue = models.FloatField(blank=True)
    evalue = models.FloatField(blank=True)
    person = models.ForeignKey(Person)
    tags = TaggableManager()
    tags_string = models.CharField(max_length="180",blank=True)

    device = models.CharField(max_length="180",choices=[("nano","nano"),("raspberry","raspberry")])


    def save(self, *args, **kwargs):



        super(TimeSeries, self).save(*args, **kwargs)

        self.tags_string = "[ "
        for i in self.tags.all():
            self.tags_string = self.tags_string + i.name + " "

        self.tags_string = self.tags_string + "]"

        super(TimeSeries, self).save(*args, **kwargs)



# TimeSeries.objects.filter(pub_date__range=(start_date, end_date))





admin.site.register(TimeSeries)
admin.site.register(Person)

