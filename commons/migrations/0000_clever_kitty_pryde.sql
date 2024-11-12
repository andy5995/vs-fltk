CREATE TABLE `apps` (
	`id` integer PRIMARY KEY AUTOINCREMENT NOT NULL,
	`app_id` blob NOT NULL,
	`base` text NOT NULL,
	`notes` text,
	`source` text
);
--> statement-breakpoint
CREATE UNIQUE INDEX `apps_app_id_unique` ON `apps` (`app_id`);--> statement-breakpoint
CREATE TABLE `cache` (
	`id` integer PRIMARY KEY AUTOINCREMENT NOT NULL,
	`hash` blob NOT NULL,
	`timestamp_creation` integer NOT NULL,
	`timestamp_expiring` integer NOT NULL,
	`source` text NOT NULL,
	`content` blob NOT NULL
);
--> statement-breakpoint
CREATE UNIQUE INDEX `cache_hash_unique` ON `cache` (`hash`);--> statement-breakpoint
CREATE TABLE `permanent` (
	`id` integer PRIMARY KEY NOT NULL,
	`app` integer NOT NULL,
	`key` text NOT NULL,
	`value` blob NOT NULL,
	FOREIGN KEY (`app`) REFERENCES `apps`(`id`) ON UPDATE cascade ON DELETE cascade
);
--> statement-breakpoint
CREATE UNIQUE INDEX `permanentUniqueIdx` ON `permanent` (`app`,`key`);--> statement-breakpoint
CREATE TABLE `policies` (
	`id` integer PRIMARY KEY AUTOINCREMENT NOT NULL,
	`app_id` blob,
	`data` text NOT NULL
);
--> statement-breakpoint
CREATE UNIQUE INDEX `policies_app_id_unique` ON `policies` (`app_id`);